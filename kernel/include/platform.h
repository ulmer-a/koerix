// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <arch/asm.h>
#include <offsets.h>

#ifdef X86
// platform-native debug print function
// in this case: qemu and bochs 0xe9 port
extern void qemu_print(const char* str);
#endif

void init_drivers();
