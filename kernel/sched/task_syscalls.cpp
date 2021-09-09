#include <syscalls.h>
#include <scheduler.h>
#include <user_task.h>
#include <user_proc.h>

void sys_exit(int status)
{
  debug() << "exit(" << status << ")\n";
  auto userTask = sched::currentUserTask();
  userTask->getProcess().exit(status);
}

void sys_thread_exit(void* ret)
{
  auto userTask = sched::currentUserTask();
  debug() << "tid " << userTask->tid() << ": thread_exit(" << ret << ")\n";
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

ssize_t sys_thread_create(void* func, void* arg, int flags)
{
  auto& proc = sched::currentUserTask()->getProcess();
  return proc.addTask(func);
}

size_t sys_thread_count()
{
  return sched::currentUserTask()->getProcess().threadCount();
}
