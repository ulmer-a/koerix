#pragma once

#include <types.h>

#define PAGE_SHIFT 12
#define PAGE_SIZE 4096

class AddrSpace
{
  public:
    AddrSpace() = default;
    ~AddrSpace() = default;

    static void setup();

    void apply();

    void map(size_t virt, size_t phys);
    void unmap(size_t virt);

  private:
    size_t m_pml4;
};
