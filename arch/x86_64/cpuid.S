// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

.section .text

.global _cpuid
_cpuid:
    mov %rdx, %r8
    mov %rdi, %rax
    cpuid
    mov %ecx, (%rsi)
    mov %edx, (%r8)
    ret
