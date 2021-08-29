#pragma once

static inline void hlt()
{
    __asm__ volatile ("hlt");
}