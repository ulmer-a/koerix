#pragma once

enum _sysinfo_type
{
  SYSINFO_MEM_SIZE,
  SYSINFO_MEM_USABLE,
  SYSINFO_MEM_USED,
  SYSINFO_THREAD_CNT,
  SYSINFO_PROC_CNT,
};

#define __NEED_struct_timespec
#define __NEED_time_t
#include <bits/alltypes.h>
