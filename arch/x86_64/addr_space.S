// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

.section .text

.global enableNx
enableNx:
    mov $0xc0000080, %rcx
    rdmsr
    btsl $11, %eax
    wrmsr
    mov $1, %rax
    ret

.global irqEnabled
irqEnabled:
    pushfq
    mov (%rsp), %rax
    shr $9, %rax
    and $1, %rax
    add $0x08, %rsp
    ret
