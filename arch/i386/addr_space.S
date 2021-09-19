// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

.section .text

.global enableNx
enableNx:
    mov $0xc0000080, %ecx
    rdmsr
    btsl $11, %eax
    wrmsr
    mov $1, %eax
    ret

.global enablePagingAndJump
enablePagingAndJump:
    // enable Physical Address Extensions (PAE)
    // by setting PAE bit in cr4
    mov %cr4, %eax
    btsl $5, %eax
    mov %eax, %cr4

    // enable Paging by setting the PG bit in cr0
    mov %cr0, %eax
    btsl $31, %eax
    mov %eax, %cr0

    // then jump to the high half
    pop %eax
    add $0x80000000, %eax
    jmp *%eax
