#pragma once

#include <types.h>

#define PAGE_SHIFT 12
#define PAGE_SIZE 4096

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

    void apply();

    void map(size_t virt, size_t phys, int flags);
    void unmap(size_t virt);

  private:
    size_t m_pml4;
};
