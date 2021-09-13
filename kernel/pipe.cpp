// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <syscalls.h>
#include <errno.h>
#include <fs/file.h>
#include <fs/fd.h>
#include <user_task.h>
#include <user_proc.h>
#include <scheduler.h>
#include <queue.h>

class Pipe : public fs::File
{
  public:
    Pipe() = default;

    fs::FileDesc getFd() {
      return fs::FileDesc(this);
    }

  protected:
    ssize_t read(char* buf, size_t len) final {
      return m_buffer.read(buf, len);
    }

    ssize_t write(char* buf, size_t len) final {
      return m_buffer.write(buf, len);
    }

    void closed() final {
      delete this;
    }

  private:
    Queue m_buffer;
};

int sys_pipe(int* fds)
{
  auto& proc = sched::currentUserTask()->getProcess();

  auto pipe = new Pipe();
  fds[0] = proc.insertOpenFile(pipe->getFd());
  fds[1] = proc.insertOpenFile(pipe->getFd());

  return 0;
}
