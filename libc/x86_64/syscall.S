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
    call _sys_errno
    ret
.endmacro

syscall_wo_ret    _Exit               0
syscall_wo_ret    thread_exit         1 
syscall_w_errno   read                2
syscall_w_errno   write               3
syscall_w_errno   close               4
syscall_w_errno   __lseek             5
syscall           set_thread_ptr      6
syscall           get_thread_ptr      7
syscall_w_errno   _ioctl              8
