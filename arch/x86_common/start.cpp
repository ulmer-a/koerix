// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <types.h>
#include <platform.h>
#include <debug.h>
#include <addr_space.h>
#include <scheduler.h>
#include <x86/stivale.h>
#include <kernel_task.h>
#include <lib/string.h>


// setup stack allocation
static const size_t SETUP_STACK_SIZE = 1024*16;
static uint8_t _INIT _ALIGNED(16) setup_stack[SETUP_STACK_SIZE];
stivale_struct* s_stivale;

_SECTION(".stivalehdr")
static struct stivale_header stivale_hdr = {
  // location of the setup stack
  .stack = (size_t)setup_stack + SETUP_STACK_SIZE,

  // BIT(0) will ask for a graphical framebuffer
  // BIT(3) all pointers received from the bootloader
  //   will be higher-half pointers.
 #ifdef i386
  .flags = BIT(0),
 #else
  .flags = BIT(0) | BIT(3),
 #endif

  // let the bootloader decide the framebuffer size
  .framebuffer_width = 0,
  .framebuffer_height = 0,
  .framebuffer_bpp = 0,

  // entry point as in the corresponding ELF field
  .entry_point = 0
};
extern void kernel_init(const char* cmdline);
static void setup_task(void* cmdline) {
  kernel_init((char*)cmdline);
}

extern "C" void setup_gdt();
extern "C" void x86_irq_init();
extern void initHeap();
extern void create_page_bitmap();

bool s_sseEnabled = false;

extern char _bss_start;
extern char _bss_end;

extern void debug_init();

extern "C" void _NORETURN _start(struct stivale_struct *stivale)
{
  memset((void*)&_bss_start, 0, (size_t)_bss_end - (size_t)_bss_start);
  debug_init();

  s_stivale = stivale;
  debug(BOOT) << "Hello, world!\n";

  /* Load our own Global Descriptor Table */
  debug(BOOT) << "Setup: Global Descriptor Table\n";
  setup_gdt();

  /* Create a map of free pages from the stivale
   * memory map. */
  create_page_bitmap();

  /* Load an interrupt descriptor table */
  debug(BOOT) << "Setup: Interrupt Descriptor Table\n";
  x86_irq_init();

  /* Create the kernel virtual memory address space and
   * setup the kernel heap */
  AddrSpace::setup();
#ifndef i386
  initHeap();

  /* Create kernel setup task and enable scheduling */
  sched::setup(new KernelTask(
      setup_task, (void*)stivale->cmdline));
  sched::yield();
#endif

  /* This will never be reached */
  for (;;) { hlt(); }
}
