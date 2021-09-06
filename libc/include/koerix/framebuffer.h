#pragma once

#define __NEED_size_t
#define __NEED_ssize_t
#include <bits/alltypes.h>

struct __fb_struct
{
  size_t width;
  size_t height;
  size_t pitch;
  size_t bpp;
  void* fb;
};

typedef struct {
    unsigned int magic;
    unsigned int version;
    unsigned int headersize;
    unsigned int flags;
    unsigned int numglyph;
    unsigned int bytesperglyph;
    unsigned int height;
    unsigned int width;
    unsigned char glyphs;
} __attribute__((packed)) psf_font_t;


extern const psf_font_t* __fb_current_font;
extern struct __fb_struct __fb_info;

#define FB_WIDTH  (__fb_info.width)
#define FB_HEIGHT (__fb_info.height)
#define FB_PITCH  (__fb_info.pitch)
#define FB_BPP    (__fb_info.bpp)
#define FB_PTR    (__fb_info.fb)

int fb_init();

#define framebuffer(x, y) *(((uint32_t*)FB_PTR) + (FB_PITCH * (y)) + (x))
#define color(r, g, b)  ( \
       (b & 0xff)         \
    | ((g & 0xff) << 8)   \
    | ((r & 0xff) << 16)  \
)

void fb_putc(size_t x, size_t y, char c);
