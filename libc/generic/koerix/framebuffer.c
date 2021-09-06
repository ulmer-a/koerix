#include <koerix/framebuffer.h>
#include <stdint.h>

enum fb_info_types
{
  FB_INFO_POINTER  = 1,
  FB_INFO_WIDTH    = 2,
  FB_INFO_HEIGHT   = 3,
  FB_INFO_PITCH    = 4,
  FB_INFO_BPP      = 5
};

struct __fb_struct __fb_info;
const psf_font_t* __fb_current_font;
uint32_t __fb_fg_color;
extern psf_font_t __koerix_console_font;

extern ssize_t fb_info(int type);

int fb_init()
{
  ssize_t ptr;
  if ((ptr = fb_info(FB_INFO_POINTER)) < 0)
    return (int)ptr;

  __fb_info.fb = (void*)ptr;
  __fb_info.width = (size_t)fb_info(FB_INFO_WIDTH);
  __fb_info.height = (size_t)fb_info(FB_INFO_HEIGHT);
  __fb_info.pitch = (size_t)fb_info(FB_INFO_PITCH);
  __fb_info.bpp = (size_t)fb_info(FB_INFO_BPP);

  __fb_current_font = &__koerix_console_font;
  return 0;
}


void fb_putc(size_t x, size_t y, char c)
{
  const psf_font_t* font = __fb_current_font;
  const size_t bytes_per_line = (font->width + 7)/8;
  const size_t index = (c < font->numglyph) ? c : 0;
  unsigned char* glyph = (unsigned char*)font +
      font->headersize + index * font->bytesperglyph;

  for (int i = 0; i < font->height; i++)
  {
    for (int j = 0; j < font->width; j++)
    {
      uint32_t glyph_word = *glyph;
      if (glyph_word & (1 << (font->width - j - 1))) {
        framebuffer(x + j, y + i) = __fb_fg_color;
      }
    }
    glyph += bytes_per_line;
  }
}

void fb_set_fg(uint32_t color)
{
  __fb_fg_color = color;
}

void fb_draw_rect(size_t x, size_t y, size_t width, size_t height)
{
  for (size_t i = 0; i < width; i++)
  {
    for (size_t j = 0; j < height; j++)
    {
      framebuffer(x + i, y + j) = __fb_fg_color;
    }
  }
}
