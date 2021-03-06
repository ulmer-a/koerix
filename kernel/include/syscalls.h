// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

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
int sys_close(int fd);
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
int sys_pipe(int* fds);
int sys_dup2(int oldfd, int newfd);
int sys_execve(const char* pathname,
               char *const argv[], char *const envp[]);
int sys_open(const char* pathname, int flags);


#ifdef __cplusplus
}
#endif
