.section .text

// TODO: properly check if supported!
.global enable_nx
enable_nx:
    mov $0xc0000080, %rcx
    rdmsr
    btsl $11, %eax
    wrmsr
    mov $1, %rax
    ret

.global sse_enable
sse_enable:
    // check if SSE is supported
    mov $0x01, %rax
    cpuid
    test $(1 << 25), %rdx
    jnz .sse_ok
    xor %rax, %rax
    ret
  .sse_ok:
    // if supported, enable right away
    mov %cr0, %rax
    btsq $1, %rax   // set monitor co-processor
    btrq $2, %rax   // clear emulation
    btsq $3, %rax   // set TS
    btsq $5, %rax   // set native exception
    mov %rax, %cr0
    mov %cr4, %rax
    btsq $9, %rax   // enable FXSAVE, FXSTOR
    btsq $10, %rax  // enable SSE exception
    mov %rax, %cr4
    mov $1, %rax
    ret

.global resetFpuFlag
resetFpuFlag:
    mov %cr0, %rax
    btsq $3, %rax
    mov %rax, %cr0
    ret

.global enableFpuCalls
enableFpuCalls:
    mov %cr0, %rax
    btrq $3, %rax
    mov %rax, %cr0
    ret
