#include <bitmap.h>
#include <string.h>

using namespace baselib;

Bitmap::Bitmap(size_t size, void* mem)
    : m_size(size)
{
    if (mem == nullptr)
    {
        m_isHeapAllocated = true;
        m_bitmap = new uint8_t[BITMAP_BYTE_SIZE(size)];
    }
    else
    {
        m_bitmap = (uint8_t*)mem;
    }
}

Bitmap::~Bitmap()
{
    if (m_isHeapAllocated)
        delete m_bitmap;
}

void Bitmap::setAll()
{
    memset((void*)m_bitmap, 1, BITMAP_BYTE_SIZE(m_size));
}

void Bitmap::clrAll()
{
    memset((void*)m_bitmap, 0, BITMAP_BYTE_SIZE(m_size));
}

void Bitmap::set(size_t n)
{
    assert(n < m_size);
    const size_t byte = n >> 3;
    const size_t bit = n % 8;
    m_bitmap[byte] |= BIT(bit);
}

void Bitmap::clr(size_t n)
{
    assert(n < m_size);
    const size_t byte = n >> 3;
    const size_t bit = n % 8;
    m_bitmap[byte] &= ~BIT(bit);
}

bool Bitmap::get(size_t n) const
{
    assert(n < m_size);
    const size_t byte = n >> 3;
    const size_t bit = n % 8;
    return (m_bitmap[byte] & BIT(bit)) == 1;
}
