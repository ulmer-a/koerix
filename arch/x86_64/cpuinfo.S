.section .text

.global _cpuid
_cpuid:
    mov %rdx, %r8
    mov %rdi, %rax
    cpuid
    mov %ecx, (%rsi)
    mov %edx, (%r8)
    ret
