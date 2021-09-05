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
