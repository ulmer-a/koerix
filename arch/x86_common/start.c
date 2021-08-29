#include <ktypes.h>
#include <platform.h>
#include <x86/stivale.h>

// setup stack allocation
static const size_t SETUP_STACK_SIZE = 8192;
static uint8_t _INIT setup_stack[SETUP_STACK_SIZE];

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

void _NORETURN _start(struct stivale_struct *stivale_info)
{
    nprint("Hello, world!\n");

    for (;;) {
        // Panic
        hlt();
    }
}