.section .text

.global enable_nx
enable_nx:
    mov $0xc0000080, %rcx
    rdmsr
    btsl $11, %eax
    wrmsr
    ret
