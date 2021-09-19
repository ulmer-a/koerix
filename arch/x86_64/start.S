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
_start_1:
    test %rcx, %rcx
    jz _start_2
    mov %rdx, (%rax)
    dec %rcx
    inc %rax
    jmp _start_1

_start_2:
    // setup global descriptor table
    callq setup_gdt

    // enable NX bit
    mov $0xc0000080, %rcx
    rdmsr
    btsl $11, %eax
    wrmsr

    // call C++ entry point
    mov %r12, %rdi
    jmp x86_init

