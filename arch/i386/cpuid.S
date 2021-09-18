.section .text

.global _cpuid
_cpuid:
    mov 4(%esp), %eax
    cpuid
    mov 8(%esp), %eax
    mov %ecx, (%eax)
    mov 12(%esp), %eax
    mov %edx, (%eax)
    ret
