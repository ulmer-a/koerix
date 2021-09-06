#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <koerix/framebuffer.h>

const psf_font_t* console_font;
size_t term_width, term_height;
size_t pos_x, pos_y;

static void term_puts(const char* str)
{
  char c;
  while ((c = *str++)) {
    if (c == '\n') {
      pos_x = 0;
      pos_y += 1;
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

const char* banner =
"\n     _  __               _         ___  ____\n"
"    | |/ /___   ___ _ __(_)_  __  / _ \\/ ___|\n"
"    | ' // _ \\ / _ \\ '__| \\ \\/ / | | | \\___ \\\n"
"    | . \\ (_) |  __/ |  | |>  <  | |_| |___) |\n"
"    |_|\\_\\___/ \\___|_|  |_/_/\\_\\  \\___/|____/\n\n\n" ;

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
    term_puts(banner);

    fb_set_fg(color(0xff, 0xff, 0xff));
    term_puts("Koerix OS Console\n"
              "Copyright (C) 2017-2021 Alexander Ulmer\n\n");

    term_puts("Welcome from the User-mode console driver program! I am "
              "an ELF program\nwith access to a full C library loaded by "
              "the kernel. My stack\nand data is NX-protected and SSE is "
              "supported.");

    fb_set_fg(color(0xff, 0x00, 0xa0));
    fb_draw_rect(600, 300, 100, 120);

    return 0;
}

