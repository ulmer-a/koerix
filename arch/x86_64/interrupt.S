// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

/* UlmerOS x86_64 interrupt entry point
 * Copyright (C) 2021 Alexander Ulmer
 *
 * Upon any interrupt, the processor will call the
 * 'save_context' label and all registers will be
 * pushed onto the kernel stack of the current thread.
 * the x86_irq_handler() function will be called with
 * a pointer to the context_t structure of the saved
 * context. the function must return such a pointer to
 * restore the given context and jump back to the
 * corresponding thread.
 */

.section .text

save_context:
    push %fs
    push %gs
    push %rbp
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15

    mov %rsp, %rdi
    call x86_irq_handler
    cli
    mov %rax, %rsp

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax
    pop %rbp
    pop %gs
    pop %fs
    add $0x10, %rsp
    iretq

.macro m_irq_handler
.global irq\@
irq\@:
    .if (\@!=8) && (\@!=17) && (\@!=30) && (\@<10 || \@>14)
    pushq $0
    .else
    nop
    nop
    .endif
    pushq $\@
    jmp save_context
.endm

.rept 48
m_irq_handler
.endr

.global irq_syscall
irq_syscall:
    pushq $0
    pushq $0x80
    jmp save_context
