#include <syscalls.h>
#include <pagemap.h>
#include <errno.h>

enum _sysinfo_type
{
  SYSINFO_MEM_SIZE,
  SYSINFO_MEM_USABLE,
  SYSINFO_MEM_USED,
};


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
  default:
    return -ENOTSUP;
  }

  return 0;
}
