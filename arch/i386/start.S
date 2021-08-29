/*
 *  Ulmer OS boot code for x86/i386
 *  Copyright (C) 2017-2021 Alexander Ulmer
 */

.section .text.init

jmp kernel_start

.balign 4
multiboot_header:
    .long 0x1badb002        // magic
    .long 0x2               // flags: flat binary
    .long -0x1badb004       // depends on flags!!!
    .long (multiboot_header)
    .long 0x100000          // load addr
    .long 0                 // whole image
    .long 0                 // don't clear BSS
    .long 0x100000          // entry addr


.global kernel_start
kernel_start:
    
    // setup a stack
    mov $0x200000, %esp

    // save multiboot struct address
    push %ebx

    // enable CPU cache
    mov %cr0, %eax
    and $0x9FFFFFFF, %eax
    mov %eax, %cr0

    // jump to C code
    jmp kernel_main
