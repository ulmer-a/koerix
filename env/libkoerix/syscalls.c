#include <koerix/syscall.h>

int errno;

#define HANDLE_ERRNO(r) \
  if ((r) < 0) {        \
    errno = -(r);       \
    return -1;          \
  } else {              \
    return (r);         \
  }

void exit(int status)
{
  syscall_int(SYS_EXIT);

  /* safeguard, will never be reached */
  for (;;) { }
}

size_t getpid()
{
  return (size_t)syscall_int(SYS_GETPID);
}

ssize_t read(int fd, char* buffer, size_t len)
{
  ssize_t ret = syscall_int(SYS_READ, fd, buffer, len);
  HANDLE_ERRNO(ret);
}

ssize_t write(int fd, char* buffer, size_t len)
{
  ssize_t ret = syscall_int(SYS_WRITE, fd, buffer, len);
  HANDLE_ERRNO(ret);
}
