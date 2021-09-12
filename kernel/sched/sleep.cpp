#include <syscalls.h>
#include <scheduler.h>
#include <task.h>

int sys_nanosleep(const struct timespec *req, struct timespec *rem)
{
  // timer tick takes 20ms at the moment.
  const uint64_t ticks = (req->tv_sec * 50) + (req->tv_nsec / 20000000);
  sched::currentTask()->sleepTicks(ticks);
  return 0;
}
