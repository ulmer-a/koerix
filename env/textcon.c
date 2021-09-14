#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <koerix/sysinfo.h>
#include <koerix/scheduler.h>
#include <koerix/framebuffer.h>
#include <koerix/threads.h>
#include <koerix/keyboard.h>
#include <fcntl.h>
#include <assert.h>

size_t term_width, term_height;
size_t pos_x, pos_y;

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
    }

    fb_putc(pos_x * __fb_current_font->width,
      pos_y * __fb_current_font->height, c, COLOR_WHITE, COLOR_BLACK);
    pos_x += 1;

    if (pos_x == term_width)
    {
      pos_x = 0;
      pos_y += 1;
    }
  }
}

static void drawText(size_t x, size_t y, const char* str,
                     uint32_t fg_color, uint32_t bg_color)
{
  size_t x_offset = 0;
  while (*str)
  {
    char c = *str++;
    fb_putc(x + x_offset, y, c, fg_color, bg_color);
    x_offset += __fb_current_font->width;
  }
}

const char* banner =
"\n     _  __               _         ___  ____\n"
"    | |/ /___   ___ _ __(_)_  __  / _ \\/ ___|\n"
"    | ' // _ \\ / _ \\ '__| \\ \\/ / | | | \\___ \\\n"
"    | . \\ (_) |  __/ |  | |>  <  | |_| |___) |\n"
"    |_|\\_\\___/ \\___|_|  |_/_/\\_\\  \\___/|____/\n\n\n";
const char* banner2 =
"Koerix OS Console\n"
"Copyright (C) 2017-2021 Alexander Ulmer\n\n";

static void memText(char* buffer, size_t bytes)
{
  size_t num;
  const char* postfix;
  if (bytes < 2048) // 2 KB
  {
    num = bytes;
    postfix = "B";
  }
  else if (bytes < 0x00100000ul) // 1MB
  {
    num = bytes >> 10;
    postfix = "KB";
  }
  else if (bytes < 0x80000000ul) // 2 GB
  {
    num = bytes >> 20;
    postfix = "MB";
  }
  else
  {
    num = bytes >> 30;
    postfix = "GB";
  }

  sprintf(buffer, "%lu %s", num, postfix);
}

static const uint32_t barBackgroundColor = color(0x43, 0x43, 0x43);

static void drawInfoBar()
{
  /* query system information to be printed */
  size_t total_mem, usable_mem, used_mem,
      thread_count, proc_count;
  sysinfo(SYSINFO_MEM_SIZE, &total_mem);
  sysinfo(SYSINFO_MEM_USABLE, &usable_mem);
  sysinfo(SYSINFO_MEM_USED, &used_mem);
  sysinfo(SYSINFO_PROC_CNT, &proc_count);
  sysinfo(SYSINFO_THREAD_CNT, &thread_count);

  /* clear the top bar */
  fb_draw_rect(2, barBackgroundColor, 0, 0, FB_WIDTH, 30, 1);

  /* draw used-memory progress bar */
  float prog = (float)used_mem / (float)usable_mem;
  size_t bytes_used = total_mem * prog;
  fb_draw_progressbar(prog, color(0xff, 0, 0),
    5, 5, FB_WIDTH/4, 20);

  /* draw memory usage text */
  char memTextBuffer1[16];
  char memTextBuffer2[16];
  memText(memTextBuffer1, used_mem);
  memText(memTextBuffer2, usable_mem);

  /* draw threads count */
  char textBuffer[64];
  sprintf(textBuffer, "%s / %s  | %lu processes | %lu threads",
          memTextBuffer1, memTextBuffer2, proc_count, thread_count);
  drawText(FB_WIDTH/4 + 20, 7, textBuffer, COLOR_WHITE, barBackgroundColor);
}

static void infoBarThread()
{
  for (;;)
  {
    drawInfoBar();
    sleep(1);
  }
}

static void stdoutThread(int shell_stdout_fd)
{
  term_width = FB_WIDTH / __fb_current_font->width;
  term_height = FB_HEIGHT / __fb_current_font->height;
  pos_x = 0, pos_y = 3;

  ssize_t len;
  char buffer[1024];
  for (;;)
  {
    len = read(shell_stdout_fd, buffer, 1024);
    if (len < 0) {
      fprintf(stderr, "textcon error: stdoutThread(): %s\n",
              strerror(errno));
      exit(EXIT_FAILURE);
    }

    term_puts(buffer, len);
  }
}

static void stdinThread(int shell_stdin_fd)
{
  if (kbd_init() < 0) {
    fprintf(stderr, "cannot open keyboard device: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  for (;;)
  {
    char c = kbd_getchar();
    write(shell_stdin_fd, &c, 1);
  }
}

static void textcon(size_t readFd, size_t writeFd)
{
  /* initialize the framebuffer */
  if (fb_init() < 0) {
    fprintf(stderr, "textcon: error: framebuffer not supported\n");
    exit(EXIT_FAILURE);
  }
  printf("initialized framebuffer: %ldx%ld\n", FB_WIDTH, FB_HEIGHT);

  /* create a thread that maintains the info bar on top
   * of the screen displaying memory usage, etc. */
  thread_create(infoBarThread, NULL, 0);

  /* create a thread that reads the pipe for data coming from
   * the shell, and prints it to the framebuffer console */
  thread_create(stdoutThread, (void*)readFd, 0);

  stdinThread(writeFd);
}

int main(int argc, char* argv[])
{
  /* get the name of the shell to be executed from the
   * command line arguments. if none, then take the default. */
  const char* shell = "/bin/sh"; // default
  if (argc > 1)
    shell = argv[1];

  /* create a IPC pipe to be able to communicate with
   * the shell. */
  int toShell[2];
  pipe(toShell);
  int toTextcon[2];
  pipe(toTextcon);

  /* fork off a new process that will execute the shell */
  pid_t pid = fork();
  if (pid == 0)
  {
    /* reroute standard input/output streams to the pipe
     * we just created. that way, when the shell thinks it
     * reads and writes from/to stdio, it really actually
     * writes into the pipe file descriptors. */
    dup2(toShell[0], 0);
    dup2(toTextcon[1], 1);
    dup2(toTextcon[1], 2);

    char lineBuffer[128];
    while (1)
    {
      fprintf(stderr, "test shell >> ");
      char c;
      char* linePtr = lineBuffer;
      while ((c = getchar()) != '\n' && c != EOF && c != '\r') {
        putc(c, stderr);
        *linePtr++ = c;
      }
      putc('\n', stderr);
      *linePtr = 0;

      fprintf(stderr, "Your command was '%s'\n", lineBuffer);
    }


    /* execute the shell */
    //static char* const shell_args[] = {
    //  "/bin/sh",
    //  NULL
    //};
    //execv("/bin/sh", shell_args);

    /* execv() returns only on error, so handle it */
    //fprintf(stderr, "error: execv(): %s\n", strerror(errno));
    //exit(EXIT_FAILURE);
  }

  /* ... while this process will handle the text console */
  textcon(toTextcon[0], toShell[1]);
  return 0;
}

