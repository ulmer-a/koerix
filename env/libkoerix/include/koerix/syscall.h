#pragma once

#include <koerix/types.h>

#define SYS_EXIT 1
#define SYS_READ 2
#define SYS_WRITE 3
#define SYS_GETPID 4

ssize_t syscall_int(size_t sys_no, ...);
void* syscall_ptr(size_t sys_no, ...);
