// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <platform.h>
#include <x86/ports.h>

#ifdef DEBUG

#define COM2_PORT 0x2f8

void debug_print_init()
{
  // setup COM2 port as debug output
  outb(COM2_PORT + 1, 0x00);  // IRQ disable
  outb(COM2_PORT + 3, 0x80);  // DLAB
  outb(COM2_PORT + 0, 1);     // Baud 115200
  outb(COM2_PORT + 1, 0);     // ...
  outb(COM2_PORT + 3, 0x03);  // 8N1
  outb(COM2_PORT + 2, 0x67);  // enable FIFO
  outb(COM2_PORT + 4, 0x0f);  // RTS/DTR
}

static void debug_putchar(char c)
{
  while ((inb(COM2_PORT + 5) & 0x20) == 0);
  outb(COM2_PORT, c);
}

void debug_print(const char* str)
{
  char c;
  while ((c = *str++))
  {
    if (c == '\n')
      debug_putchar('\r');
    debug_putchar(c);
  }
}

#endif
