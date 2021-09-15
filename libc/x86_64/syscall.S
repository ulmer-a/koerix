// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: MIT

.section .text

.macro syscall name id
  .global \name
  \name:
    mov $\id, %rax
    int $0x80
    ret
.endmacro

.macro syscall_wo_ret name id
  .global \name
  \name:
    mov $\id, %rax
    int $0x80
.endmacro

.macro syscall_w_errno name id
  .global \name
  \name:
    mov $\id, %rax
    int $0x80
    mov %rax, %rdi
    call _sys_errno
    ret
.endmacro

.global yield
yield:
    int $0x1f
    ret

#include "../syscall_list.h"
