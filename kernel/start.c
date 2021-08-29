#include <platform.h>

void __attribute__((noreturn)) kernel_main()
{
    nprint("Hello, world!\n");

    hlt();
    for (;;) {}
}