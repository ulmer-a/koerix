#include <syscalls.h>
#include <user_task.h>
#include <scheduler.h>

void sys_exit(int status)
{
  debug() << "exit(" << status << ")\n";
  sys_thread_exit((void*)(size_t)status);
}

void sys_thread_exit(void* ret)
{
  auto userTask = sched::currentUserTask();
  debug() << "tid " << userTask->tid() << ": thread_exit(" << ret << ")\n";
  sched::currentUserTask()->exit();
}
