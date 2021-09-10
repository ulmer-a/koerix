#pragma once

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

void sys_exit(int status);
void sys_thread_exit(void* ret);
ssize_t sys_read(int fd, char* buffer, size_t len);
ssize_t sys_write(int fd, char* buffer, size_t len);
int close(int fd);
void set_thread_ptr(void* ptr);
void* get_thread_ptr();
ssize_t fb_info(int type);
ssize_t sys_thread_create(void* rt, void* func, void* arg, int flags);
size_t sys_thread_count();

#ifdef __cplusplus
}
#endif
