// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#define MIN_LOAD_ADDR     0x100000ul
#define FRAMEBUFFER_ADDR  0x0000400000000000ul
#define USER_BREAK        0x0000800000000000ul
#define IDENT_OFFSET      0xffff800000000000ul
#define EXIT_ADDR         0xffff8000deadbeeful
#define KHEAP_START       0xffffff0000000000ul
#define KHEAP_END         0xffffffff7ffffffful
#define CODE_OFFSET       0xffffffff80000000ul
#define KERNEL_CODE       0xffffffff80100000ul

#define PAGE_SHIFT        12
#define PAGE_SIZE         4096
