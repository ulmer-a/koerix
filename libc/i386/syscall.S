// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: MIT

.section .text

.macro syscall32
    push %ebx
    push %esi
    push %edi
    push %ebp
    mov 0x10(%esp), %ebx
    mov 0x14(%esp), %ecx
    mov 0x18(%esp), %edx
    mov 0x1c(%esp), %edi
    mov 0x20(%esp), %esi
    mov 0x24(%esp), %ebp
    int $0x80
    pop %ebp
    pop %edi
    pop %esi
    pop %ebx
.endmacro

.macro syscall name id
  .global \name
  \name:
    mov $\id, %eax
    syscall32
    ret
.endmacro

.macro syscall_wo_ret name id
  .global \name
  \name:
    mov $\id, %eax
    syscall32
.endmacro

.macro syscall_w_errno name id
  .global \name
  \name:
    mov $\id, %eax
    syscall32
    push %eax
    call _sys_errno
    add $4, %esp
    ret
.endmacro

.global yield
yield:
    int $0x1f
    ret

#include "../syscall_list.h"
