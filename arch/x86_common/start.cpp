#include <types.h>
#include <platform.h>
#include <debug.h>
#include <addr_space.h>
#include <scheduler.h>
#include <x86/stivale.h>
#include <x86/features.h>
#include <kernel_task.h>
#include <string.h>

// setup stack allocation
static const size_t SETUP_STACK_SIZE = 1024*16;
static uint8_t _INIT setup_stack[SETUP_STACK_SIZE];
stivale_struct* s_stivale;

_SECTION(".stivalehdr")
static struct stivale_header stivale_hdr = {
  // location of the setup stack
  .stack = (size_t)setup_stack + SETUP_STACK_SIZE,

  // BIT(0) will ask for a graphical framebuffer
  // BIT(3) all pointers received from the bootloader
  //   will be higher-half pointers.
  .flags = BIT(0) | BIT(3),

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
  debug() << "Hello, world!\n";

  /* Load our own Global Descriptor Table */
  debug() << "Setting up Global Descriptor Table ...";
  setup_gdt();
  debug() << " done\n";

  /* Create a map of free pages from the stivale
   * memory map. */
  create_page_bitmap();

  /* Load an interrupt descriptor table */
  x86_irq_init();

  /* Create the kernel virtual memory address space and
   * setup the kernel heap */
  AddrSpace::setup();
  initHeap();

  /* enable SSE and AVX if available */
  s_sseEnabled = sse_enable();
  debug() << "trying to enable SSE: "
          << (s_sseEnabled ? "ok\n" : "not available\n");
  s_sseEnabled = sse_enable();

  /* Create kernel setup task and enable scheduling */
  sched::setup(new KernelTask(
      setup_task, (void*)stivale->cmdline));
  sched::yield();

  /* This will never be reached */
  for (;;) { hlt(); }
}
