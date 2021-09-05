.section .text

.global _exit
_exit:
  xor %rax, %rax
  int $0x80

.global thread_exit
thread_exit:
  mov $1, %rax
  int $0x80

.global read
read:
  mov $2, %rax
  int $0x80
  call _sys_errno
  ret

.global write
write:
  mov $3, %rax
  int $0x80
  call _sys_errno
  ret
