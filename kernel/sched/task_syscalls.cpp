// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <syscalls.h>
#include <scheduler.h>
#include <user_task.h>
#include <user_proc.h>
#include <lib/errno.h>

void sys_exit(int status)
{
  debug(PROCESS) << "exit(" << status << ")\n";
  auto userTask = sched::currentUserTask();
  userTask->getProcess().exit(status);
}

void sys_thread_exit(void* ret)
{
  auto userTask = sched::currentUserTask();
  debug(TASK) << "tid " << userTask->tid() << ": thread_exit(" << ret << ")\n";
  sched::currentUserTask()->exit();
}

void set_thread_ptr(void* ptr)
{
  sched::currentUserTask()->setThreadPtr(ptr);
}

void* get_thread_ptr()
{
  return sched::currentUserTask()->getThreadPtr();
}

ssize_t sys_thread_create(void* rt0, void* func, void* arg, int flags)
{
  auto& proc = sched::currentUserTask()->getProcess();
  auto tid = proc.addTask(rt0, func, arg);

  debug(PROCESS) << "PID " << proc.pid()
                 << ": created new thread with TID " << tid << "\n";
  return tid;
}

size_t sys_thread_count()
{
  return sched::currentUserTask()->getProcess().threadCount();
}

ssize_t sys_fork()
{
  return sched::currentUserTask()->getProcess().fork();
}

size_t sys_getpid()
{
  return sched::currentUserTask()->getProcess().pid();
}
