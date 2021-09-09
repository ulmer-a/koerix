#include <koerix/threads.h>
#include "../threads/pthread_impl.h"
#include <stdlib.h>

extern ssize_t _thread_create(void* func, void* arg, int flags);
extern void _thread_exit(void* ret);

ssize_t thread_create(void* func, void* arg, int flags)
{
  ssize_t tid = _thread_create(func, arg, flags);
  if (tid < 0)
    return tid;

  thread_info_t* t_info = malloc(sizeof(thread_info_t));
  t_info->tid = tid;
  set_thread_ptr(t_info);
  return tid;
}

void thread_exit(void* ret)
{
  free(__pthread_self());
  _thread_exit(ret);
}

size_t thread_id()
{
  return __pthread_self()->tid;
}
