#include <types.h>
#include <mm.h>
#include <debug.h>

extern "C" void __cxa_pure_virtual()
{
  assert(false);
}

void *__dso_handle;

extern "C" int __cxa_atexit(void (*destructor) (void *), void *arg, void *dso)
{
  return 0;
}

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
