#include <koerix/framebuffer.h>

enum fb_info_types
{
  FB_INFO_POINTER  = 1,
  FB_INFO_WIDTH    = 2,
  FB_INFO_HEIGHT   = 3,
  FB_INFO_PITCH    = 4
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

  return 0;
}
