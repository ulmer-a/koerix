#pragma once

/* page frame allocation */
#include <pagemap.h>

/* default placement operator new */
inline void* operator new (size_t, void* p) noexcept	{ return p; }
inline void* operator new[] (size_t, void* p) noexcept	{ return p; }

/* heap allocation */
#ifdef DEBUG
  #define kmalloc(n) _kmalloc((n), __LINE__, __func__)
  void* _kmalloc(size_t n, unsigned line, const char* function);
#else
  void* kmalloc(size_t n);
#endif
void kfree(void* ptr);
