#include <bitmap.h>
#include <debug.h>
#include <string.h>

using namespace baselib;

Bitmap::Bitmap(size_t size, void* mem)
    : m_size(size)
{
    if (mem == nullptr)
    {
        m_isHeapAllocated = true;
        m_bitmap = new uint8_t[BITMAP_BYTE_SIZE(size)];
        clrAll();
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
    memset((void*)m_bitmap, 0xff, BITMAP_BYTE_SIZE(m_size));
    m_setCount = m_size;
}

void Bitmap::clrAll()
{
    memset((void*)m_bitmap, 0, BITMAP_BYTE_SIZE(m_size));
    m_setCount = 0;
}

void Bitmap::set(size_t n)
{
    assert(n < m_size);
    const size_t byte = n >> 3;
    const size_t bit = n % 8;

    uint8_t tmp = m_bitmap[byte];
    m_bitmap[byte] |= BIT(bit);

    if (tmp != m_bitmap[byte])
        m_setCount++;
}

void Bitmap::clr(size_t n)
{
    assert(n < m_size);
    const size_t byte = n >> 3;
    const size_t bit = n % 8;

    uint8_t tmp = m_bitmap[byte];
    m_bitmap[byte] &= ~BIT(bit);

    if (tmp != m_bitmap[byte])
        m_setCount--;
}

size_t Bitmap::getClrCount() const
{
    return m_size - m_setCount;
}

size_t Bitmap::getSetCount() const
{
    return m_setCount;
}

bool Bitmap::get(size_t n) const
{
    assert(n < m_size);
    const size_t byte = n >> 3;
    const size_t bit = n % 8;
    return (m_bitmap[byte] & BIT(bit)) == 1;
}
