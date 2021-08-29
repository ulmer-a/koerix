#include <platform.h>
#include <x86/ports.h>

void nprint(const char* str)
{
    while (*str)
        outb(0xe9, *str++);
}