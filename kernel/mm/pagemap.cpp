#include <pagemap.h>

PageMap::PageMap()
    : m_pagemap(nullptr)
{
    // Default constructor shouldn't be used
    assert(false);
}

PageMap::PageMap(size_t pagemapSize, size_t freePages,
                 uint8_t* initializedPagemap)
    : m_totalPages(pagemapSize)
    , m_freePageCount(freePages)
    , m_pagemap(initializedPagemap)
{
}
