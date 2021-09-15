// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

extern "C" void cli();
extern "C" void sti();

static inline void hlt()
{
  __asm__ volatile ("wfi;");
}

extern "C" void _yield();

extern "C" bool irqEnabled();
