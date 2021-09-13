// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <syscalls.h>
#include <errno.h>
#include <debug.h>
#include <scheduler.h>
#include <user_task.h>
#include <user_proc.h>
#include <term.h>

ssize_t sys_read(int fd, char* buffer, size_t len)
{
  auto& proc = sched::currentUserTask()->getProcess();
  auto fdObj = proc.getOpenFile(fd);

  if (!fdObj.valid())
    return -EBADF;

  return fdObj.read(buffer, len);
}

ssize_t sys_write(int fd, char* buffer, size_t len)
{
  auto& proc = sched::currentUserTask()->getProcess();
  auto fdObj = proc.getOpenFile(fd);

  if (!fdObj.valid())
    return -EBADF;

  return fdObj.write(buffer, len);
}

int sys_close(int fd)
{
  auto& proc = sched::currentUserTask()->getProcess();
  if (!proc.closeFile(fd))
    return -EBADF;
  return 0;
}

int sys_dup2(int oldfd, int newfd)
{
  if (oldfd < 0 || newfd < 0)
    return -EINVAL;

  auto& proc = sched::currentUserTask()->getProcess();
  auto fd = proc.getOpenFile(oldfd);

  if (!fd.valid())
    return -EBADF;

  proc.insertOpenFile(fd, newfd);
  return 0;
}
