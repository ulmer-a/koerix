#pragma once

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

void sys_exit(int status);
void sys_thread_exit(void* ret);

ssize_t sys_read(int fd, char* buffer, size_t len);
ssize_t sys_write(int fd, char* buffer, size_t len);

#ifdef __cplusplus
}
#endif
