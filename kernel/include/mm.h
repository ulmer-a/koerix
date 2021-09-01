#pragma once

/* page frame allocation */
#include <pagemap.h>

/* default placement operator new */
inline void* operator new (size_t, void* p) noexcept	{ return p; }
inline void* operator new[] (size_t, void* p) noexcept	{ return p; }

/* heap allocation */
void* kmalloc(size_t n);
void kfree(void* ptr);
