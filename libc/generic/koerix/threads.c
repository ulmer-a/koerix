#include <koerix/threads.h>
#include "../threads/pthread_impl.h"
#include <stdlib.h>

extern ssize_t _thread_create(void* rt_ptr, void* func, void* arg, int flags);

static void thread_rt0(void*(*func)(void*), void* arg, size_t tid)
{
  /* allocate thread info structure on the thread's stack */
  thread_info_t thread_info;
  thread_info.tid = tid;
  set_thread_ptr(&thread_info);

  void* ret = func(arg);
  thread_exit(ret);
}

ssize_t thread_create(void* func, void* arg, int flags)
{
  return _thread_create(thread_rt0, func, arg, flags);
}

size_t thread_id()
{
  return __pthread_self()->tid;
}
