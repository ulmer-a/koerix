// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

/* kernel initialization function */
extern void kernel_init(const char* cmdline);
static void setup_task(void* cmdline) {
  kernel_init((char*)cmdline);
}

extern "C" void x86_irq_init();
extern void initHeap();
extern void create_page_bitmap();
extern void debug_init();
