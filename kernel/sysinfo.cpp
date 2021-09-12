#include <syscalls.h>
#include <pagemap.h>
#include <errno.h>
#include <proc_list.h>
#include <scheduler.h>
#include <x86/features.h>
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
