#pragma once

#include <types.h>
#include <bits.h>

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
ssize_t sys_fork();
size_t sys_getpid();
ssize_t get_sched_task_info(size_t tid, void* sched_task_info);
int sys_sysinfo(int type, size_t* value);
int sys_nanosleep(const struct timespec *req, struct timespec *rem);

#ifdef __cplusplus
}
#endif
