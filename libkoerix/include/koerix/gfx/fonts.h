#pragma once

typedef struct
{
  unsigned int magic;
  unsigned int version;
  unsigned int headersize;
  unsigned int flags;
  unsigned int numglyph;
  unsigned int bytesperglyph;
  unsigned int height;
  unsigned int width;
} fb_font_t;

int fb_load_font(void* psf_font);
