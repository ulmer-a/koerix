// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <arch/asm.h>
#include <offsets.h>

#ifdef DEBUG
extern void debug_print_init();
extern void debug_print(const char* str);
#endif

void init_drivers();
