#include <types.h>
#include <mm.h>

void* operator new (size_t n)
{
    return kmalloc(n);
}

void* operator new[] (size_t n)
{
    return kmalloc(n);
}

void  operator delete (void* p) noexcept
{
    kfree(p);
}

void  operator delete[] (void* p) noexcept
{
    kfree(p);
}

void* kmalloc(size_t n)
{
    assert(false);
    return nullptr;
}

void kfree(void* ptr)
{
    assert(false);
}
