#include <koerix/framebuffer.h>
#include <koerix/scheduler.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void print_htop()
{
    ssize_t tid = 0;
    sched_task_info_t ts;
    printf("Tid\tState\n");
    while ((tid = get_sched_task_info(tid, &ts)) >= 0)
    {
       printf("%d\t%s\n", tid,
              ts.state == SCHED_TASK_RUNNING ? "running" :
              ts.state == SCHED_TASK_SLEEPING ? "sleeping" :
                                                "killed");
    }
}

int main()
{
    pid_t pid = fork();
    if (pid == 0) {
      printf("hello from the child with PID %d\n", getpid());
      while(1)
          yield();
      exit(0);
    }


    printf("welcome from my framebuffer test\n");

    for(int count = 0; count < 500; count++) {
        if ((count % 50) == 0)
            print_htop();
        yield();
    }

    fb_init();
    for (size_t x = 0; x < FB_WIDTH; x++) {
        for (size_t y = 0; y < FB_HEIGHT; y++) {
            framebuffer(x, y) = color(x % 0xff, y % 0xff, y % 0xff);
        }
    }

    return 0;
}

