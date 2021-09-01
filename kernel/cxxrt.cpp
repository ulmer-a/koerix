#include <types.h>

extern "C" void __cxa_pure_virtual()
{
  assert(false);
}

void *__dso_handle;

extern "C" int __cxa_atexit(void (*destructor) (void *), void *arg, void *dso)
{
  return 0;
}
