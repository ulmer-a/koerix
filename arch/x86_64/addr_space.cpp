#include <addr_space.h>
#include <debug.h>
#include <string.h>
#include <mm.h>
#include <arch/asm.h>
#include <x86/features.h>


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
  uint64_t available2                : 10;
  uint64_t no_exec                   : 1;
} _PACKED;

struct AddrSpace::Mapping
{
  /* indices, pointers and PPN numbers
   * of all four tables, where
   *  0.. page table
   *  1.. page directory
   *  2.. pdpt
   *  (3.. pml4)   */
  size_t                ppn[4];
  size_t                indices[4];
  GenericPagingTable*   tables[4];

  void*                 page;
};


AddrSpace s_kernelAddrSpace;
bool s_initialized = false;
bool s_nxEnabled = false;

AddrSpace::AddrSpace()
{
  /* Create a new address space by allocating
   * a fresh Page Map Level 4 */
  m_pml4 = PageMap::get().alloc();

  updateKernelMappings();
}

AddrSpace* AddrSpace::clone()
{
  /* nobody should modify the structure of the address
   * space while we're cloning it. */
  ScopedMutex smtx { m_lock };

  /* create a new AddrSpace with kernel mappings by using
   * the default constructor. */
  auto forked = new AddrSpace();

  /* get pointers to both the old and new PML4 */
  auto pml4_old = (GenericPagingTable*)PPN_TO_VIRT(this->m_pml4);
  auto pml4_new = (GenericPagingTable*)PPN_TO_VIRT(forked->m_pml4);

  auto& pageMap = PageMap::get();
  for (size_t i = 0; i < 256; i++)
  {
    if (!pml4_old[i].present)
      continue;

    /* clear write permissions for the entire address spaces of both
     * the old and new processes. processes that like to write on a
     * page must create their own copies and make them writable until
     * everyone has their own copy. */
    if (pml4_old[i].cow_was_writable == 0)
      pml4_old[i].cow_was_writable = pml4_old[i].write;
    pml4_old[i].write = 0;
    pml4_new[i] = pml4_old[i];

    /* increase the reference count for the respective PDPT table */
    pageMap.addRef(pml4_old[i].ppn);
  }

  /* it's important that the TLB will be flushed after setting all
   * the tables readonly by calling apply(). */
  this->apply();
  return forked;
}

void AddrSpace::setup()
{
    assert(sizeof(GenericPagingTable) == 8);

    /* Create a new virtual address space for the kernel */
    debug() << "allocating kernel virtual address space\n";
    auto kernelSpace = new (&s_kernelAddrSpace) AddrSpace();

    /* map the whole physical memory to the upper half */
    debug() << "mapping physical memory to the upper half...";
    const size_t totalPages = PageMap::get().getTotalPageCount();
    const size_t ident_page_offset = IDENT_OFFSET >> PAGE_SHIFT;
    for (size_t page = 0; page < totalPages; page++)
        kernelSpace->map(page + ident_page_offset, page, MAP_WRITE);
    debug() << " done\n";

    /* Enable support for NX pages */
    s_nxEnabled = enable_nx();
    debug() << "trying to enable NX: "
            << (s_nxEnabled ? "ok\n" : "not available\n");

    /* Switch to the newly created address space. */
    kernelSpace->apply();
    s_initialized = true;
    debug() << "successfully written %cr3\n";
}

AddrSpace &AddrSpace::kernel()
{
    return s_kernelAddrSpace;
}

void AddrSpace::apply()
{
    /* Switch to the virtual address space represented
     * by this class */
    void* phys_addr = (void*)(m_pml4 << PAGE_SHIFT);
    __asm__ volatile ("mov %0, %%cr3;" : : "r"(phys_addr));
}

void AddrSpace::invalidate(size_t virt)
{
  /* only do the TLB invalidation if we're mapping/unmapping
   * the currently active address space. */
  size_t cr3;
  __asm__ volatile ("movq %%cr3, %0;" : "=r"(cr3));
  if (cr3 == (m_pml4 << PAGE_SHIFT))
  {
    /* Invalidate the respective entry in the TLB */
    virt <<= PAGE_SHIFT;
    __asm__ volatile ("invlpg (%0)" : : "b"(virt) : "memory");
  }
}

void AddrSpace::map(size_t virt, size_t phys, int flags)
{
  ScopedMutex smtx { m_lock };
  auto& pageMap = PageMap::get();

  /* first, pre-compute all the indices into the different
   * page tables. for example tableIndices[3] will be the
   * index into the PML4 table, whereas tableIndex[0] will
   * be the index into the page table. */
  size_t tableIndices[4];
  tableIndices[3] = (virt >> 27) & 0x1ff;     // PML4
  tableIndices[2] = (virt >> 18) & 0x1ff;     // PDPT
  tableIndices[1] = (virt >> 9) & 0x1ff;      // PDIR
  tableIndices[0] = (virt) & 0x1ff;           // PTAB

  size_t currentTablePPN = m_pml4;
  for (int level = 3; level >= 0; level--)
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
        if (flags & MAP_NOEXEC) currentLevelTableEntry.no_exec = 1;
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
  ScopedMutex smtx { m_lock };
  auto& pageMap = PageMap::get();

  Mapping mapping;
  resolve(virt, mapping);

  /* if the payload page is actually mapped, free it */
  if (mapping.page != nullptr)
    pageMap.free(mapping.ppn[0]);

  for (int level = 0; level < 4; level++)
  {
    /* check if the table on the current level is actually present */
    if (mapping.tables[level] == nullptr)
      continue;

    auto& entry = mapping.tables[level][mapping.indices[level]];
    *((uint64_t*)&entry) = 0;

    if (level == 3)
      break;

    /* if there are other present entries, we cannot release
     * any further tables -> we're done. */
    if (checkForPresentEntries(mapping.tables[level]))
      break;

    pageMap.free(mapping.ppn[level + 1]);
  }

  invalidate(virt);
}

bool AddrSpace::triggerCow(size_t virt)
{
  ScopedMutex smtx { m_lock };
  auto& pageMap = PageMap::get();

  size_t indices[4];
  indices[3] = (virt >> 27) & 0x1ff;
  indices[2] = (virt >> 18) & 0x1ff;
  indices[1] = (virt >> 9) & 0x1ff;
  indices[0] = (virt) & 0x1ff;

  debug() << "COW triggered for addr "
          << (void*)(virt << PAGE_SHIFT) << "\n";

  size_t currentTablePpn = m_pml4;
  for (int level = 3; level >= 0; level--)
  {
    auto& currentLevelTableEntry = ((GenericPagingTable*)
        PPN_TO_VIRT(currentTablePpn))[indices[level]];

    if (!currentLevelTableEntry.present)
    {
      /* we're done, it's not a cow fault */
      return false;
    }

    if (level == 0 && currentLevelTableEntry.write == 0 &&
        currentLevelTableEntry.cow_was_writable == 0)
    {
      /* the page is not writable, so the page fault is legit */
      return false;
    }

    pageMap.lock();
    if (pageMap.getRefs(currentLevelTableEntry.ppn) > 1)
    {
      // create new table one level below
      // e.g. if PML4 is currentLevelTable -> create new PDPT
      const size_t new_table = pageMap.alloc(true);
      auto new_table_ptr = (GenericPagingTable*)
          PPN_TO_VIRT(new_table);

      // copy from the parent's table, then set both
      // tables non_writable and the new table to not_copied
      auto old_table_ptr = (GenericPagingTable*)
          PPN_TO_VIRT(currentLevelTableEntry.ppn);

      if (level == 0)
      {
        /* if we're in level 0, that means currentLevelTable
         * is a PageTable and thus the level below is an
         * actual table. That means we must not set any bits
         * but just copy the page */
        memcpy(new_table_ptr, old_table_ptr, PAGE_SIZE);
      }
      else
      {
        for (size_t i = 0; i < 512; i++)
        {
          auto& oldEntry = old_table_ptr[i];
          auto& newEntry = new_table_ptr[i];

          if (!oldEntry.present)
          {
            newEntry.present = 0;
            continue;
          }

          if (oldEntry.cow_was_writable == 0)
          {
            /* if the old entry was already copied, writable
             * might be zero, hereby causing cow_writable to go
             * zero, even though the page is actually writable.
             * this was REALLY nasty to debug! */
            oldEntry.cow_was_writable = oldEntry.write;
          }
          oldEntry.write = 0;
          newEntry = oldEntry;
          pageMap.addRef(oldEntry.ppn);
        }
      }

      /* decrease the reference count of the page that we just copied */
      pageMap.free(currentLevelTableEntry.ppn);
      pageMap.unlock();

      currentLevelTableEntry.ppn = new_table;
      currentLevelTableEntry.write = 1;
    }
    else
    {
      pageMap.unlock();

      if (currentLevelTableEntry.write == 0 &&
          currentLevelTableEntry.cow_was_writable)
      {
        currentLevelTableEntry.write = 1;
      }
    }

    currentTablePpn = currentLevelTableEntry.ppn;
  }

  return true;
}

void AddrSpace::updateKernelMappings()
{
  if (!s_initialized)
    return;

  /* copy upper half of PML4 entries from the kernel's address space
   * over to this address space. */
  auto& kernelVspace = kernel();
  auto my_upper_pml4 = (GenericPagingTable*)PPN_TO_VIRT(m_pml4) + 256;
  auto kernel_upper_pml4 =
      (GenericPagingTable*)PPN_TO_VIRT(kernelVspace.m_pml4) + 256;
  memcpy((void*)my_upper_pml4, (void*)kernel_upper_pml4, PAGE_SIZE / 2);
}

void AddrSpace::resolve(size_t virt, AddrSpace::Mapping& mapping)
{
  size_t& pml4i = (mapping.indices[3] = (virt >> 27) & 0x1ff);     // PML4
  size_t& pdpti = (mapping.indices[2] = (virt >> 18) & 0x1ff);     // PDPT
  size_t& pdiri = (mapping.indices[1] = (virt >> 9) & 0x1ff);      // PDIR
  size_t& ptbli = (mapping.indices[0] = (virt) & 0x1ff);           // PTAB

  auto pml4 = (GenericPagingTable*)PPN_TO_VIRT(m_pml4);
  auto pdpt = (GenericPagingTable*)PPN_TO_VIRT(mapping.ppn[3] = pml4[pml4i].ppn);
  auto pdir = (GenericPagingTable*)PPN_TO_VIRT(mapping.ppn[2] = pdpt[pdpti].ppn);
  auto ptbl = (GenericPagingTable*)PPN_TO_VIRT(mapping.ppn[1] = pdir[pdiri].ppn);
  mapping.page = (void*)PPN_TO_VIRT(mapping.ppn[0] = ptbl[ptbli].ppn);

  mapping.tables[3] = pml4;
  mapping.tables[2] = pdpt;
  mapping.tables[1] = pdir;
  mapping.tables[0] = ptbl;
}

bool AddrSpace::checkForPresentEntries(GenericPagingTable* table)
{
  for (size_t i = 0; i < 512; i++)
  {
    if (table[i].present)
      return true;
  }
  return false;
}
