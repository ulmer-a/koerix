// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later
//
// Entry point into the kernel
//

.section .text

.global _start
_start:
    // save the pointer to stivale_struct
    mov %rdi, %r12

    // clear BSS section
    movabsq $_bss_end, %rax
    mov %rax, %rcx
    movabsq $_bss_start, %rax
    sub %rax, %rcx
    xor %rdx, %rdx
    s1: test %rcx, %rcx
    jz s2
    mov %rdx, (%rax)
    dec %rcx
    inc %rax
    jmp s1

    // setup global descriptor table
    s2: callq setup_gdt

    // enable NX bit
    mov $0xc0000080, %rcx
    rdmsr
    btsl $11, %eax
    wrmsr

    // call C++ entry point
    mov %r12, %rdi
    jmp x86_init
