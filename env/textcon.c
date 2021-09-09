#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <koerix/framebuffer.h>

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

const char* banner =
"\n     _  __               _         ___  ____\n"
"    | |/ /___   ___ _ __(_)_  __  / _ \\/ ___|\n"
"    | ' // _ \\ / _ \\ '__| \\ \\/ / | | | \\___ \\\n"
"    | . \\ (_) |  __/ |  | |>  <  | |_| |___) |\n"
"    |_|\\_\\___/ \\___|_|  |_/_/\\_\\  \\___/|____/\n\n\n";
const char* banner2 =
"Koerix OS Console\n"
"Copyright (C) 2017-2021 Alexander Ulmer\n\n";

int main() {
    printf("hello, world!\n");

    // initialize framebuffer
    if (fb_init() < 0) {
      fprintf(stderr, "error: framebuffer not supported\n");
      return 1;
    }

    printf("framebuffer: %ld x %ld, pitch=%ld\n",
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
    }

    return 0;
}

