#include <syscalls.h>
#include <errno.h>
#include <debug.h>
#include <scheduler.h>
#include <user_task.h>
#include <user_proc.h>
#include <term.h>

ssize_t sys_read(int fd, char* buffer, size_t len)
{
  if (fd > 2)
    return -EBADF;
  return sched::currentUserTask()->getProcess()
      .getTerm().read(buffer, len);
}

ssize_t sys_write(int fd, char* buffer, size_t len)
{
  if (fd > 2)
    return -EBADF;
  return sched::currentUserTask()->getProcess()
      .getTerm().write(buffer, len);
}
