#include <stdio.h>
#include <koerix/sysinfo.h>
#include <koerix/framebuffer.h>
#include <koerix/threads.h>
#include <koerix/scheduler.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

static void drawInfoBar()
{
  size_t total_mem, usable_mem, used_mem;
  sysinfo(SYSINFO_MEM_SIZE, &total_mem);
  sysinfo(SYSINFO_MEM_USABLE, &usable_mem);
  sysinfo(SYSINFO_MEM_USED, &used_mem);

  memset(FB_PTR, 0, 30 * FB_PITCH * FB_BPP);

  float prog = (float)used_mem / (float)usable_mem;
  size_t bytes_used = total_mem * prog;
  fb_draw_progressbar(prog, color(0xff, 0, 0),
    5, 5, FB_WIDTH/4, 20);
}

static void* func(void* thread)
{
  size_t tid = thread_id();
  fprintf(stderr, "thread %lu here\n", tid);
  if (tid > 500)
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
  fb_init();

  pid_t pid = fork();
  if (pid == 0)
  {
    thread_create(func, NULL, 0);
    while (thread_count() > 1)
    {
      yield();
    }
  }
  else
  {
    for (;;)
    {
      drawInfoBar();
      yield();
    }
  }

  return 0;
}
