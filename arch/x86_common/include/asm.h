// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>

static inline void cli()
{
  __asm__ volatile ("cli");
}

static inline void sti()
{
  __asm__ volatile ("sti");
}

static inline void hlt()
{
  __asm__ volatile ("hlt");
}

static inline void arch_yield()
{
  /* yield() will trigger interrupt 0x1f. this will
   * cause the interrupt handler to run schedule()
   * inside an interrupt context, but without the
   * timer being fired, as it would if we just
   * triggered the timer irq at this location. */
  __asm__ volatile ("int $0x1f;");
}

void preemt_disable();
void preemt_enable();
