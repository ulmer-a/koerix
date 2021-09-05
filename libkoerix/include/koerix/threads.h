#pragma once

#include <sys/types.h>

typedef size_t koerix_thread_t;

koerix_thread_t thread_create(void* func, void* arg, int flags);
void thread_exit(void* ret);
