#pragma once

#define __NEED_size_t
#include <bits/alltypes.h>

enum _sysinfo_type
{
  SYSINFO_MEM_SIZE,
  SYSINFO_MEM_USABLE,
  SYSINFO_MEM_USED,
};

int sysinfo(int type, size_t* value);
