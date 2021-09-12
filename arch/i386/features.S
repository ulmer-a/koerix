// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

.section .text

// TODO: properly check if supported!
.global enable_nx
enable_nx:
    mov $0xc0000080, %ecx
    rdmsr
    btsl $11, %eax
    wrmsr
    mov $1, %eax
    ret

.global sse_enable
sse_enable:
    // check if SSE is supported
    mov $0x01, %eax
    cpuid
    test $(1 << 25), %edx
    jnz .sse_ok
    xor %eax, %eax
    ret
  .sse_ok:
    // if supported, enable right away
    mov %cr0, %eax
    btsl $1, %eax   // set monitor co-processor
    btrl $2, %eax   // clear emulation
    btsl $3, %eax   // set TS
    btsl $5, %eax   // set native exception
    mov %eax, %cr0
    mov %cr4, %eax
    btsl $9, %eax   // enable FXSAVE, FXSTOR
    btsl $10, %eax  // enable SSE exception
    mov %eax, %cr4
    mov $1, %eax
    ret

.global resetFpuFlag
resetFpuFlag:
    mov %cr0, %eax
    btsl $3, %eax
    mov %eax, %cr0
    ret

.global enableFpuCalls
enableFpuCalls:
    mov %cr0, %eax
    btrl $3, %eax
    mov %eax, %cr0
    ret
