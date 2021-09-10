#pragma once

#include <sys/types.h>

ssize_t thread_create(void* func, void* arg, int flags);
void _Noreturn thread_exit(void* ret);

size_t thread_count();
size_t thread_id();

/* the set_thread_ptr() and get_thread_ptr() system calls allow
 * saving a per-thread pointer in the kernel. it is used by libc
 * to refer to per-thread data.  */
void* get_thread_ptr();
void set_thread_ptr(void* ptr);

