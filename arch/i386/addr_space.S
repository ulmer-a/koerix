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
