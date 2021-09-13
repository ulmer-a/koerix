#include <stdio.h>
#include <user_task.h>
#include <user_proc.h>
#include <scheduler.h>
#include <term.h>

StdIO* StdIO::s_stdio = nullptr;

StdIO& StdIO::get()
{
  if (s_stdio == nullptr)
    s_stdio = new StdIO();
  return *s_stdio;
}

fs::FileDesc StdIO::getFd()
{
  return fs::FileDesc(this, true);
}

ssize_t StdIO::read(char* buf, size_t len)
{
  auto term = sched::currentUserTask()->getProcess().getTerm();
  return term.read(buf, len);
}

ssize_t StdIO::write(char* buf, size_t len)
{
  auto term = sched::currentUserTask()->getProcess().getTerm();
  return term.write(buf, len);
}
