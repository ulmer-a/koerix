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
    push %ds
    push %es
    push %fs
    push %gs
    push %ebp
    push %eax
    push %ebx
    push %ecx
    push %edx
    push %esi
    push %edi

    push %esp
    call x86_irq_handler
    cli
    add $4, %esp

    pop %edi
    pop %esi
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    pop %ebp
    pop %gs
    pop %fs
    pop %es
    pop %ds
    add $8, %esp
    iretl

.macro m_irq_handler
.global irq\@
irq\@:
    .if (\@!=8) && (\@!=17) && (\@!=30) && (\@<10 || \@>14)
    pushl $0
    .else
    nop
    nop
    .endif
    pushl $\@
    jmp save_context
.endm

.rept 48
m_irq_handler
.endr

.global irq_syscall
irq_syscall:
    pushl $0
    pushl $0x80
    jmp save_context

.global irqEnabled
irqEnabled:
    pushfl
    mov (%esp), %eax
    shr $9, %eax
    and $1, %eax
    add $0x04, %esp
    ret
