#pragma once
#include <syscalls.h>

static void* syscalls[] = {
  (void*)sys_exit,          // 0
  (void*)sys_thread_exit,   // 1
  (void*)sys_read,          // 2
  (void*)sys_write,         // 3
  nullptr,                  // 4
  nullptr,                  // 5
  (void*)set_thread_ptr,    // 6
  (void*)get_thread_ptr,    // 7
  nullptr,                  // 8
  (void*)fb_info,           // 9
  (void*)sys_thread_create, // 10
  (void*)sys_thread_count,  // 11
  (void*)sys_fork,          // 12
  (void*)sys_getpid         // 13
};
