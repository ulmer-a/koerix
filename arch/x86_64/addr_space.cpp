#include <addr_space.h>
#include <debug.h>
#include <string.h>
#include <mm.h>
#include <arch/asm.h>


struct GenericPagingTable
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

AddrSpace s_kernelAddrSpace;
bool s_initialized = false;

AddrSpace::AddrSpace()
{
    /* Create a new address space by allocating
     * a fresh Page Map Level 4 */
    m_pml4 = PageMap::get().alloc();

    updateKernelMappings();
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
    enable_nx();

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
    /* Switch to the virtual address space represented by this class */
    void* phys_addr = (void*)(m_pml4 << PAGE_SHIFT);
    __asm__ volatile ("mov %0, %%cr3;" : : "r"(phys_addr));
}

static inline void tlb_invalidate(size_t virt)
{
    /* Invalidate the respective entry in the TLB */
    virt <<= PAGE_SHIFT;
    __asm__ volatile ("invlpg (%0)" : : "b"(virt) : "memory");
}

void AddrSpace::map(size_t virt, size_t phys, int flags)
{
    auto pageMap = PageMap::get();

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
        auto& currentLevelTableEntry =
            ((GenericPagingTable*)PPN_TO_VIRT(currentTablePPN))[tableIndices[level]];

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
  assert(false);
}

void AddrSpace::updateKernelMappings()
{
  if (!s_initialized)
    return;

  /* copy upper half of PML4 entries from the kernel's address space
   * over to this address space. */
  auto kernelVspace = kernel();
  auto my_upper_pml4 = (GenericPagingTable*)PPN_TO_VIRT(m_pml4) + 256;
  auto kernel_upper_pml4 =
      (GenericPagingTable*)PPN_TO_VIRT(kernelVspace.m_pml4) + 256;
  memcpy((void*)my_upper_pml4, (void*)kernel_upper_pml4, PAGE_SIZE / 2);
}
