#include <syscalls.h>
#include <errno.h>

ssize_t sys_read(int fd, char* buffer, size_t len)
{
  return -ENOTSUP;
}

ssize_t sys_write(int fd, char* buffer, size_t len)
{
  return -ENOTSUP;
}
