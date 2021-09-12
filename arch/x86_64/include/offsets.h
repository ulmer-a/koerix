// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#define KHEAP_START 0xffffffff00000000
#define KHEAP_END   0xffffffffffffffff

#define PAGE_SHIFT 12
#define PAGE_SIZE 4096

#define MIN_LOAD_ADDR   0x100000ul
#define EXIT_ADDR       0xffff8000deadbeeful
#define IDENT_OFFSET    0xffff800000000000ul
#define USER_BREAK      0x0000800000000000ul

#define FRAMEBUFFER_ADDR  0x0000400000000000ul
