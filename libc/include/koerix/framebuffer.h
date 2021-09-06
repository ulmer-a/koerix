#pragma once

#define __NEED_size_t
#define __NEED_ssize_t
#include <bits/alltypes.h>

struct __fb_struct
{
  size_t width;
  size_t height;
  size_t pitch;
  void* fb;
};

extern struct __fb_struct __fb_info;

#define FB_WIDTH  (__fb_info.width)
#define FB_HEIGHT (__fb_info.height)
#define FB_PITCH  (__fb_info.pitch)

int fb_init();
