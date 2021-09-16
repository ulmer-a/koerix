// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <syscalls.h>
#include <pagemap.h>
#include <lib/errno.h>
#include <proc_list.h>
#include <scheduler.h>
#include <bits.h>


int sys_sysinfo(int type, size_t* value)
{
  auto& pageMap = PageMap::get();
  switch (type)
  {
  case SYSINFO_MEM_SIZE:
    *value = pageMap.getTotalMemory();
    break;
  case SYSINFO_MEM_USABLE:
    *value = pageMap.getUsableMemory();
    break;
  case SYSINFO_MEM_USED:
    *value = pageMap.getUsedMemory();
    break;
  case SYSINFO_PROC_CNT:
    *value = ProcList::get().procCount();
    break;
  case SYSINFO_THREAD_CNT:
    *value = sched::taskCount();
    break;
  default:
    return -ENOTSUP;
  }

  return 0;
}
