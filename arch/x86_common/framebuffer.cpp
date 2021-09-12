// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <syscalls.h>
#include <x86/stivale.h>
#include <offsets.h>
#include <scheduler.h>
#include <addr_space.h>
#include <user_task.h>
#include <user_proc.h>
#include <errno.h>

size_t fb_page_count()
{
  const size_t byte_per_px = s_stivale->framebuffer_bpp >> 3;
  const size_t line = s_stivale->framebuffer_pitch * byte_per_px;
  const size_t byte_size = line * s_stivale->framebuffer_height;

  size_t pages = byte_size >> PAGE_SHIFT;
  if ((byte_size & 0xfff) != 0)
    return pages + 1;
  return pages;
}

void* fb_map()
{
  size_t count = fb_page_count();
  size_t virt_start_page = FRAMEBUFFER_ADDR >> PAGE_SHIFT;
  size_t start_page = s_stivale->framebuffer_addr >> PAGE_SHIFT;
  auto& addrSpace = sched::currentUserTask()->getProcess().getAddrSpace();
  for (size_t i = 0; i < count; i++)
  {
    addrSpace.map(virt_start_page + i, start_page + i,
      AddrSpace::MAP_USER | AddrSpace::MAP_WRITE | AddrSpace::MAP_NOEXEC);
  }
  return (void*)FRAMEBUFFER_ADDR;
}

ssize_t fb_info(int type)
{
  switch (type)
  {
  case 1: // framebuffer pointer
    return (ssize_t)fb_map();
  case 2: // width
    return s_stivale->framebuffer_width;
  case 3: // height
    return s_stivale->framebuffer_height;
  case 4: // pitch
    return s_stivale->framebuffer_pitch / (s_stivale->framebuffer_bpp >> 3);
  case 5: // pitch
    return s_stivale->framebuffer_bpp >> 3;
  default:
    return -EINVAL;
  }
}
