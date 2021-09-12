// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#pragma once

#include <sys/types.h>

typedef struct
{
    int errno_val;
    size_t tid;
} pthread_t;
