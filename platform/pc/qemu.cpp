#include <platform.h>
#include <x86/ports.h>

void qemu_print(const char* str)
{
    while (*str)
        outb(0xe9, *str++);
}