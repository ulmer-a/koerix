#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <koerix/sysinfo.h>
#include <koerix/scheduler.h>
#include <koerix/framebuffer.h>
#include <koerix/threads.h>

const psf_font_t* console_font;
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

static void drawText(size_t x, size_t y, const char* str)
{
  size_t x_offset = 0;
  while (*str)
  {
    char c = *str++;
    fb_putc(x + x_offset, y, c);
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
  memset(FB_PTR, 0, 30 * FB_PITCH * FB_BPP);

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
  drawText(FB_WIDTH/4 + 10, 7, textBuffer);
}

static void doSth()
{
  printf("thread %ld here\n", thread_id());

  int i = 10000000;
  while (i--);

  if (thread_id() > 500)
    return;

  thread_create(doSth, NULL, 0);
  thread_create(doSth, NULL, 0);
}

int main()
{
  if (fb_init() < 0) {
    fprintf(stderr, "error: framebuffer not supported\n");
    return 1;
  }

  thread_create(doSth, NULL, 0);
  while (1)
  {
    drawInfoBar();
    yield();
  }

  /*printf("framebuffer: %ld x %ld, pitch=%ld\n",
         FB_WIDTH, FB_HEIGHT, FB_PITCH);

  console_font = __fb_current_font;
  term_width = FB_WIDTH / console_font->width;
  term_height = FB_HEIGHT / console_font->height;
  pos_x = 0;
  pos_y = 1;

  fb_set_fg(color(0xf5, 0xdf, 0x3b));
  term_puts(banner, strlen(banner));
  fb_set_fg(color(0xff, 0xff, 0xff));
  term_puts(banner2, strlen(banner2));

  fflush(stdout);

  char buffer[1024];
  for (;;)
  {
    ssize_t len = read(0, buffer, 1024);
    if (len < 0) {
      fprintf(stderr, "I/O error\n");
      return 1;
    }

    term_puts(buffer, len);
    write(1, buffer, len);
  }*/


  return 0;
}

