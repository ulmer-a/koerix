// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <platform.h>
#include <x86/ports.h>

void debug_print_init() { }

void debug_print(const char* str)
{
    while (*str)
        outb(0xe9, *str++);
}
