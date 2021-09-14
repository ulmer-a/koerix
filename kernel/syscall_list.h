// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once
#include <syscalls.h>

static void* syscalls[] = {
  (void*)sys_exit,                  // 0
  (void*)sys_thread_exit,           // 1
  (void*)sys_read,                  // 2
  (void*)sys_write,                 // 3
  (void*)sys_close,                 // 4
  nullptr,                          // 5
  (void*)set_thread_ptr,            // 6
  (void*)get_thread_ptr,            // 7
  nullptr,                          // 8
  (void*)fb_info,                   // 9
  (void*)sys_thread_create,         // 10
  (void*)sys_thread_count,          // 11
  (void*)sys_fork,                  // 12
  (void*)sys_getpid,                // 13
  (void*)get_sched_task_info,       // 14
  (void*)sys_sysinfo,               // 15
  (void*)sys_nanosleep,             // 16
  (void*)sys_pipe,                  // 17
  (void*)sys_dup2,                  // 18
  (void*)sys_execve,                // 19
  (void*)sys_open                   // 20
};
