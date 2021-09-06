#pragma once

#include <sys/types.h>

typedef size_t koerix_thread_t;

koerix_thread_t thread_create(void* func, void* arg, int flags);
void thread_exit(void* ret);

/* the set_thread_ptr() and get_thread_ptr() system calls allow
 * saving a per-thread pointer in the kernel. it is used by libc
 * to refer to per-thread data.  */
void* get_thread_ptr();
void set_thread_ptr(void* ptr);

size_t gettid();
