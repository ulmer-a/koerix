#include <koerix/threads.h>
#include <stdlib.h>
#include <stdio.h>

static void* func(void* thread)
{
  size_t tid = thread_id();
  fprintf(stderr, "thread %lu here\n", tid);
  if (tid > 250)
    return (void*)0xdeadbeef;

  // create two more threads
  thread_create(func, NULL, 0);
  thread_create(func, NULL, 0);

  // do some stuff
  int i = 10000000;
  while (i--);

  return NULL;
}

int main()
{
  thread_create(func, NULL, 0);

  while (thread_count() > 1);
  return 0;
}
