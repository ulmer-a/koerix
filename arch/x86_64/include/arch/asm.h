// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

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

static inline void _yield()
{
  /* yield() will trigger interrupt 0x1f. this will
   * cause the interrupt handler to run schedule()
   * inside an interrupt context, but without the
   * timer being fired, as it would if we just
   * triggered the timer irq at this location. */
  __asm__ volatile ("int $0x1f;");
}

extern "C" bool irqEnabled();

void printStackTrace();
