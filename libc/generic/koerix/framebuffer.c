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

  return 0;
}

void fb_putc(const psf_font_t* font, size_t x, size_t y, char c)
{
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
        framebuffer(x + j, y + i) = color(0xff, 0xff, 0xff);
      }
    }
    glyph += bytes_per_line;
  }


}
