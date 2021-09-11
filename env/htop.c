#include <koerix/framebuffer.h>
#include <koerix/scheduler.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const psf_font_t* console_font;
size_t term_width, term_height;
size_t pos_x, pos_y;

const char MoveToHome[] = "\e[H";
const char ClearScreen[] = "\e[J";

static void term_puts(const char* str, size_t len)
{
  for (size_t i = 0; i < len; i++) {
    char c = str[i];

    if (c == '\n' || c == '\r') {
      pos_x = 0;
      pos_y += 1;
      continue;
    } else if (c == '\b') {
      pos_x -= 1;
      c = ' ';
    } else if (c == '\t') {
      int remaining = 8 - (pos_x % 8);
      pos_x += remaining;
      continue;
    }

    fb_putc(pos_x * console_font->width,
      pos_y * console_font->height, c);
    pos_x += 1;

    if (pos_x == term_width)
    {
      pos_x = 0;
      pos_y += 1;
    }
  }
}


void print_htop()
{
    ssize_t next_tid = 0;
    sched_task_info_t ts;
    pos_x = pos_y = 0;
    const char header[] = "Tid\tState\n";
    //printf(ClearScreen);
    //printf(MoveToHome);
    printf(header);
    term_puts(header, strlen(header));
    do
    {
        next_tid = get_sched_task_info(next_tid, &ts);
        char consolebuffer[128];
        sprintf(consolebuffer, "%d\t%s\n", ts.tid,
                ts.state == SCHED_TASK_RUNNING ? "running" :
                ts.state == SCHED_TASK_SLEEPING ? "sleeping" :
                                                  "killed");
        printf(consolebuffer);
        term_puts(consolebuffer, strlen(consolebuffer));
    }
    while (next_tid != -1);
    const char footer[] = "------------------------";
    printf(footer);
    term_puts(footer, strlen(footer));
}

int main()
{
    pid_t pid = fork();
    if (pid == 0) {

      printf("Press any key to exit PID %d\n", getpid());
      char buf[1];
      read(0, buf, 1);
      _exit(0);
    }


    printf("welcome from my framebuffer test\n");
    fb_init();

    console_font = __fb_current_font;
    term_width = FB_WIDTH / console_font->width;
    term_height = FB_HEIGHT / console_font->height;
    pos_x = pos_y = 0;

    for(int count = 0; count < 500; count++) {
        if ((count % 50) == 0)
            print_htop();
        yield();
    }



    return 0;
}

