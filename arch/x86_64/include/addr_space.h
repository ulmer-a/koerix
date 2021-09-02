#pragma once

#include <types.h>

#define PAGE_SHIFT 12
#define PAGE_SIZE 4096

#define IDENT_OFFSET    0xffff800000000000ul
#define PPN_TO_PTR(x)   ((void*)((x) << PAGE_SHIFT))
#define PPN_TO_VIRT(x)  ((void*)(((x) << PAGE_SHIFT) + IDENT_OFFSET))

class AddrSpace
{
  public:
    enum MapFlags
    {
        MAP_USER    = BIT(0),
        MAP_WRITE   = BIT(1),
        MAP_NOEXEC  = BIT(2)
    };

    AddrSpace();
    ~AddrSpace() = default;

    static void setup();
    static AddrSpace& kernel();

    void apply();

    void map(size_t virt, size_t phys, int flags);
    void unmap(size_t virt);

  private:
    size_t m_pml4;
};
