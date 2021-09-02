#include <pagemap.h>
#include <debug.h>

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

size_t PageMap::alloc()
{
    for (size_t i = 0; i < m_totalPages; i++)
    {
        if (m_pagemap[i] == 0)
        {
            m_pagemap[i]++;
            return i;
        }
    }

    assert(false);
    return (size_t)-1;
}

void PageMap::free(size_t ppn)
{
    m_pagemap[ppn] -= 1;
}
