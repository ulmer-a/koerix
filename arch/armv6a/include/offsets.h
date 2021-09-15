#pragma once

#define KHEAP_START     0xe0000000
#define KHEAP_END       0xffffffff

#define PAGE_SHIFT      12
#define PAGE_SIZE       4096

#define MIN_LOAD_ADDR   0x00100000ul
#define EXIT_ADDR       0xdeadbeeful
#define IDENT_OFFSET    0x80000000ul
#define USER_BREAK      0x80000000ul

#define FRAMEBUFFER_ADDR  0x40000000ul
