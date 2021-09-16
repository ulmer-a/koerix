// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <pagemap.h>
#include <debug.h>
#include <sync/atomic.h>

PageMap::PageMap()
    : m_pagemap(nullptr)
{
    // Default constructor shouldn't be used
    assert(false);
}

PageMap::PageMap(size_t pagemapSize, size_t freePages,
                 size_t usablePages, uint8_t* initializedPagemap)
    : m_totalPages(pagemapSize)
    , m_usablePages(usablePages)
    , m_freePageCount(freePages)
    , m_pagemap(initializedPagemap)
    , m_usedPages(usablePages - m_freePageCount)
{
}

void PageMap::lock()
{
  m_lock.lock();
}

void PageMap::unlock()
{
  m_lock.unlock();
}

size_t PageMap::getRefs(size_t ppn)
{
  if (ppn >= m_totalPages)
    return 1;
  return m_pagemap[ppn];
}

size_t PageMap::alloc(bool noLock)
{
  if (!noLock)
    m_lock.lock();

  for (size_t i = 0; i < m_totalPages; i++)
  {
    if (m_pagemap[i] == 0)
    {
      m_pagemap[i]++;
      m_freePageCount--;
      m_usedPages++;

      if (!noLock)
        m_lock.unlock();
      return i;
    }
  }

  if (!noLock)
    m_lock.unlock();

  assert(false);
  return (size_t)-1;
}

void PageMap::addRef(size_t ppn)
{
  if (ppn >= m_totalPages)
    return;

  if (atomic_add8(&m_pagemap[ppn], 1) == 0)
  {
    m_freePageCount -= 1;
    m_usedPages += 1;
  }
}

void PageMap::free(size_t ppn)
{
  if (ppn >= m_totalPages)
    return;

  /* decrement and get the before-reference count */
  size_t before = atomic_add8(&m_pagemap[ppn], -1);

  if (before == 0)
  {
    /* if refcount before was zero, then we did some sort
     * of double free -> not good */
    assert(false);
    m_pagemap[ppn] = 0;
  }
  else if (before == 1)
  {
    m_usedPages -= 1;
    m_freePageCount += 1;
  }
}
