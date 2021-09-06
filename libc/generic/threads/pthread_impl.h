#pragma once

#include <pthread.h>
#include <assert.h>
#include <koerix/threads.h>

#define __NEED_locale_t
#include <bits/alltypes.h>

typedef struct
{
  size_t tid;
  int errno_val;
  locale_t locale;
} thread_info_t;

#define __pthread_self() ((thread_info_t*)get_thread_ptr())

static inline void __futexwait(volatile void *addr, int val, int priv)
{
    // not implemented
	assert(0);
}

static inline void __wake(volatile void *addr, int cnt, int priv)
{
    // not implemented
	assert(0);
}