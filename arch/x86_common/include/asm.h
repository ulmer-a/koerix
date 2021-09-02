#pragma once

#include <types.h>

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

extern "C" void enable_nx();

size_t xchg(size_t val, size_t* mem);
size_t atomic_add(size_t* mem, ssize_t increment);
