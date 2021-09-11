#include <sys/types.h>
#include <errno.h>
#include "threads/pthread_impl.h"

int *__errno_location(void)
{
  thread_info_t* t_info = __pthread_self();
  return &t_info->errno_val;
}

ssize_t _sys_errno(ssize_t ret)
{
  if (ret < 0)
  {
    *__errno_location() = -ret;
    return -1;
  }
  return ret;
}

weak_alias(__errno_location, ___errno_location);
