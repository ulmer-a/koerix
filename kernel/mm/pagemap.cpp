#include <pagemap.h>
#include <debug.h>

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

size_t PageMap::alloc()
{
  m_lock.lock();
  for (size_t i = 0; i < m_totalPages; i++)
  {
    if (m_pagemap[i] == 0)
    {
      m_pagemap[i]++;
      m_freePageCount--;
      m_usedPages++;
      m_lock.unlock();
      return i;
    }
  }

  m_lock.unlock();
  assert(false);
  return (size_t)-1;
}

void PageMap::free(size_t ppn)
{
  m_lock.lock();
  m_pagemap[ppn] -= 1;
  m_usedPages -= 1;
  m_freePageCount += 1;
  m_lock.unlock();
}
