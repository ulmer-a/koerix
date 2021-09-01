#pragma once

#include <types.h>
#include <bitmap.h>
#include <addr_space.h>

class PageMap
{
  public:
    PageMap();

    static PageMap& get();
    PageMap(size_t pagemapSize, size_t freePages,
            uint8_t* initializedMap);

    size_t alloc();
    void free(size_t ppn);

    uint64_t getTotalMemory() { return m_totalPages << PAGE_SHIFT; }
    uint64_t getFreeMemory() { return m_freePageCount << PAGE_SHIFT; }
    uint64_t getUsedMemory() { return (m_totalPages - m_freePageCount) << PAGE_SHIFT; }

  private:
    size_t m_totalPages;
    size_t m_freePageCount;
    uint8_t* m_pagemap;
};
