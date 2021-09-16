// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <types.h>
#include <raspi.h>
#include <debug.h>
#include <lib/string.h>

extern void debug_init();

extern char _bss_start;
extern char _bss_end;

extern "C" void _NORETURN start()
{
  memset((void*)&_bss_start, 0, (size_t)_bss_end - (size_t)_bss_start);
  debug_init();

  debug(BOOT) << "Hello, world!";

  for (;;) { }
}
