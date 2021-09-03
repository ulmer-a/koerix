#pragma once

#include <koerix/types.h>

typedef size_t koerix_thread_t;

koerix_thread_t koerix_create_thread(void*(*func)(void*), void* arg);
