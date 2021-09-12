// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#define KHEAP_START         0xe0000000ul
#define KHEAP_END           0xfffffffful

#define PAGE_SHIFT          12
#define PAGE_SIZE           4096

#define MIN_LOAD_ADDR       0x00100000ul
#define EXIT_ADDR           0xdeadbeeful
#define IDENT_OFFSET        0x80000000ul
#define USER_BREAK          0x80000000ul
#define IDENT_OFFSET        0x80000000ul

#define FRAMEBUFFER_ADDR    0x40000000ul
