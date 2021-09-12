// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <debug.h>
#include <x86/ports.h>

#define OSCILLATOR  1193182 // constant

#define PIT_CMD     0x43
#define PIT_CH0     0x40
#define PIT_CMD_CH0 0x00
#define PIT_ACCESS  0x30
#define PIT_RATEGEN 0x04

#define TIMER_IRQ   0

void timer_reset(uint16_t freqHz)
{
  debug() << "setting timer freq: " << freqHz << "Hz\n";

  uint16_t pit = OSCILLATOR / freqHz;
  outb(PIT_CMD, PIT_CMD_CH0 | PIT_ACCESS | PIT_RATEGEN);
  outb(PIT_CH0, pit & 0xff);
  outb(PIT_CH0, (pit >> 8) & 0xff);
}
