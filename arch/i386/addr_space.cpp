// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <addr_space.h>
#include <x86/cpuinfo.h>
#include <lib/string.h>

struct AddrSpace::GenericPagingTable
{
  uint64_t present                   : 1;
  uint64_t write                     : 1;
  uint64_t user                      : 1;
  uint64_t write_through             : 1;
  uint64_t cache_disable             : 1;
  uint64_t accessed                  : 1;
  uint64_t dirty                     : 1;
  uint64_t zero1                     : 2;
  uint64_t available1                : 3;
  uint64_t ppn                       : 28;
  uint64_t zero2                     : 12;
  uint64_t cow_was_writable          : 1;
  uint64_t shared                    : 1;
  uint64_t available2                : 9;
  uint64_t no_exec                   : 1;
} _PACKED;

struct AddrSpace::Mapping
{
  /* indices, pointers and PPN numbers
   * of all three tables, where
   *  0.. page table
   *  1.. page directory
   *  (2.. pdpt)  */
  size_t                ppn[3];
  size_t                indices[3];
  GenericPagingTable*   tables[3];

  void*                 page;
};


AddrSpace s_kernelAddrSpace;
bool s_initialized = false;

/* regarding NX (no execute) pages */
bool s_nxEnabled = false;
extern "C" void enableNx();

AddrSpace::AddrSpace()
{
  m_pdpt = PageMap::get().alloc();
  updateKernelMappings();

  debug(VSPACE) << "allocated new virtual address space (PDPT @ "
                << PPN_TO_PTR(m_pdpt) << ")\n";
}

AddrSpace* AddrSpace::clone()
{
  return this;
}

void AddrSpace::setup()
{
  assert(sizeof(GenericPagingTable) == 8);

  /* Create a new virtual address space for the kernel */
  auto kernelSpace = new (&s_kernelAddrSpace) AddrSpace();

  /* map the whole physical memory to the upper half */
  debug(VSPACE) << "mapping physical memory to the upper half...\n";
  const size_t totalPages = PageMap::get().getTotalPageCount();
  const size_t ident_page_offset = IDENT_OFFSET >> PAGE_SHIFT;
  for (size_t page = 0; page < totalPages; page++)
      kernelSpace->map(page + ident_page_offset, page, MAP_WRITE);

  /* Enable support for NX pages if available */
  if ((s_nxEnabled = cpuid::getFeatures3().nx))
    enableNx();
  debug(VSPACE) << "trying to enable NX: "
          << (s_nxEnabled ? "ok\n" : "not available\n");

  /* Switch to the newly created address space. */
  kernelSpace->apply();
  s_initialized = true;
  debug(VSPACE) << "successfully written %cr3\n";
}

AddrSpace& AddrSpace::kernel()
{
  return s_kernelAddrSpace;
}

void AddrSpace::apply()
{
  /* Switch to the virtual address space represented
   * by this class */
  void* phys_addr = (void*)(m_pdpt << PAGE_SHIFT);
  __asm__ volatile ("mov %0, %%cr3;" : : "r"(phys_addr));
}

void* _ppn_to_virt(size_t ppn)
{
  if (!s_initialized)
    return PPN_TO_PTR(ppn);

  assert(false);
}

void AddrSpace::map(size_t virt, size_t phys, int flags)
{
  ScopedLock smtx { m_lock };
  auto& pageMap = PageMap::get();

  /* first, pre-compute all the indices into the different
   * page tables. for example tableIndices[3] will be the
   * index into the PML4 table, whereas tableIndex[0] will
   * be the index into the page table. */
  size_t tableIndices[3];
  tableIndices[2] = (virt >> 18) & 0x3;       // PDPT
  tableIndices[1] = (virt >> 9) & 0x1ff;      // PDIR
  tableIndices[0] = (virt) & 0x1ff;           // PTAB

  size_t currentTablePPN = m_pdpt;
  for (int level = 2; level >= 0; level--)
  {
    /* Compute a reference to the entry in the current table */
    auto& currentLevelTableEntry = ((GenericPagingTable*)
        PPN_TO_VIRT(currentTablePPN))[tableIndices[level]];

    if (level == 0)
    {
      assert(!currentLevelTableEntry.present);
    }

    /* Check if an entry is present for the current table,
     * if not, we'll have to add one and allocate a new sub-table. */
    if (!currentLevelTableEntry.present)
    {
      size_t newTablePPN = phys;
      if (level != 0)
      {
        /* in case we're not yet at the page table level,
         * we need to allocate and clear a new page table
         * and clear it instead of just assigning the phys PPN. */
        newTablePPN = pageMap.alloc();
        memset(PPN_TO_VIRT(newTablePPN), 0, PAGE_SIZE);
      }

      /* clear the entry and fill with new data */
      *((uint64_t*)&currentLevelTableEntry) = 0;
      currentLevelTableEntry.present = 1;
      currentLevelTableEntry.ppn = newTablePPN;

      if (level == 0)
      {
        /* only set protections for actual pages */
        if (flags & MAP_WRITE)  currentLevelTableEntry.write = 1;
        if (flags & MAP_USER)   currentLevelTableEntry.user = 1;
        if (flags & MAP_SHARED) currentLevelTableEntry.shared = 1;

        /* only map a page NX if the features is actually available */
        if (flags & MAP_NOEXEC && s_nxEnabled)
          currentLevelTableEntry.no_exec = 1;
      }
      else
      {
        /* be generous on the upper levels */
        currentLevelTableEntry.write = 1;
        currentLevelTableEntry.user = 1;
      }
    }

    currentTablePPN = currentLevelTableEntry.ppn;
  }
}


void AddrSpace::unmap(size_t virt)
{

}


bool AddrSpace::triggerCow(size_t virt)
{
  return false;
}

AddrSpace::~AddrSpace()
{

}

void AddrSpace::updateKernelMappings()
{
  if (!s_initialized)
    return;

  /* copy upper half of PDPT entries from the kernel's address space
   * over to this address space. */
  auto& kernelVspace = kernel();
  auto my_upper_pdpt = (GenericPagingTable*)PPN_TO_VIRT(m_pdpt) + 2;
  auto kernel_upper_pdpt =
      (GenericPagingTable*)PPN_TO_VIRT(kernelVspace.m_pdpt) + 2;
  memcpy((void*)my_upper_pdpt, (void*)kernel_upper_pdpt, sizeof(uint64_t) * 2);
}
