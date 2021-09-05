#include <sys/types.h>

int _errno;

ssize_t _sys_errno(ssize_t ret)
{
  if (ret < 0)
  {
    _errno = -ret;
    return -1;
  }
  return ret;
}
