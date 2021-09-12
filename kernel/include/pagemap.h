// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>
#include <offsets.h>
#include <spin.h>

class PageMap
{
  public:
    PageMap();
    PageMap(const PageMap& self) = delete;
    PageMap& operator=(PageMap& self) = delete;

    static PageMap& get();
    PageMap(size_t pagemapSize, size_t freePages,
            size_t usablePages, uint8_t* initializedMap);

    void lock();
    void unlock();

    size_t getRefs(size_t ppn);

    size_t alloc(bool noLock = false);
    void addRef(size_t ppn);
    void free(size_t ppn);

    size_t getTotalPageCount() { return m_totalPages; }
    size_t getUsablePageCount() { return m_usablePages; }
    uint64_t getTotalMemory() { return getTotalPageCount() << PAGE_SHIFT; }
    uint64_t getUsableMemory() { return getUsablePageCount() << PAGE_SHIFT; }

    size_t getFreePageCount() { return m_usablePages - m_usedPages; }
    size_t getUsedPageCount() { return m_usedPages; }
    uint64_t getFreeMemory() { return getFreePageCount() << PAGE_SHIFT; }
    uint64_t getUsedMemory() { return getUsedPageCount() << PAGE_SHIFT; }

  private:
    Spinlock m_lock;
    size_t m_totalPages;      // absolute pages count
    size_t m_usablePages;     // usable pages count
    size_t m_freePageCount;   // absolute free pages count
    size_t m_usedPages;       // pages actually in use by OS & software
    uint8_t* m_pagemap;
};
