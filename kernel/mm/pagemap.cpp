#include <pagemap.h>

PageMap::PageMap()
    : m_bitmap(0, (void*)0x01)
{
    // Default constructor shouldn't be used
    assert(false);
}

PageMap::PageMap(size_t size, void* mem)
    : m_bitmap(size, mem)
{ }
