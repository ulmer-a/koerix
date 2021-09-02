#include <addr_space.h>
#include <debug.h>
#include <mm.h>

#define IDENT_OFFSET 0xffff800000000000ul
#define PPN_TO_PTR(x) ((void*)((x) << PAGE_SHIFT))

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

void AddrSpace::setup()
{
    assert(sizeof(GenericPagingTable) == 8);

    /* Create a new virtual address space for the kernel */
    debug() << "allocating kernel virtual address space\n";
    auto kernelSpace = new (&s_kernelAddrSpace) AddrSpace();

    /* map the whole physical memory to the upper half */
    debug() << "mapping physical memory to the upper half...";
    auto pageMap = PageMap::get();
    const size_t ident_page_offset = IDENT_OFFSET >> PAGE_SHIFT;
    for (size_t page = 0; page < pageMap.getTotalPageCount(); page++)
        kernelSpace->map(page + ident_page_offset, page);
    debug() << " done\n";

    /* Switch to the newly created address space. */
    //kernelSpace->apply();
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

void AddrSpace::map(size_t virt, size_t phys)
{
}
