#pragma once

#include <sys/types.h>

typedef struct
{
    int errno_val;
    size_t tid;
} pthread_t;