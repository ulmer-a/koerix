#pragma once

#define KHEAP_START 0xffffffff00000000
#define KHEAP_END   0xffffffffffffffff

#define PAGE_SHIFT 12
#define PAGE_SIZE 4096

#define MIN_LOAD_ADDR   0x100000ul
#define IDENT_OFFSET    0xffff800000000000ul
#define USER_BREAK      0x0000800000000000ul
