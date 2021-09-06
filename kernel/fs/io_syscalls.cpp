#include <syscalls.h>
#include <errno.h>
#include <debug.h>

ssize_t sys_read(int fd, char* buffer, size_t len)
{
  return len;
}

ssize_t sys_write(int fd, char* buffer, size_t len)
{
  return len;
}
