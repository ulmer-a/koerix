// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

.section .text

.global fpuDetectAndEnable
fpuDetectAndEnable:
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

.global fpuEnableTrap
fpuEnableTrap:
    mov %cr0, %eax
    btsl $3, %eax
    mov %eax, %cr0
    ret

.global fpuClearTrap
fpuClearTrap:
    mov %cr0, %eax
    btrl $3, %eax
    mov %eax, %cr0
    ret
