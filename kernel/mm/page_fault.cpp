/*
 * Page fault handler
 */

#include <page_fault.h>
#include <scheduler.h>
#include <debug.h>
#include <offsets.h>
#include <user_task.h>
#include <user_proc.h>
#include <loader.h>
#include <addr_space.h>
#include <pagemap.h>

static bool checkIfValid(size_t addr, FaultFlags flags)
{
  if (addr < MIN_LOAD_ADDR) {
    debug() << "PF: nullptr dereferenced!\n";
    return false;
  }

  if (addr >= USER_BREAK) {
    debug() << "PF: referencing (unmapped) kernel memory!\n";
    return false;
  }

  return true;
}

bool handlePageFault(size_t addr, FaultFlags flags)
{
  assert((flags & PF_RESERVED) == 0);

  if (!checkIfValid(addr, flags))
  {
    return false;
  }

  auto currentTask = sched::currentTask();
  assert(currentTask->isUserTask());
  auto& process = ((UserTask*)currentTask)->getProcess();

  /* check for a copy-on-write fault */
  if ((flags & PF_PRESENT) && (flags & PF_WRITE))
  {
    size_t virt = addr >> PAGE_SHIFT;
    sched::currentUserTask()->getProcess()
        .getAddrSpace().triggerCow(virt);
    return true;
  }

  if (process.isValidStackAddr(addr))
  {
    /* the address is a valid stack address, so map a page at that location
     * that can be read and written, but not executed. */
    process.getAddrSpace().map(addr >> PAGE_SHIFT, PageMap::get().alloc(),
      AddrSpace::MAP_USER | AddrSpace::MAP_WRITE | AddrSpace::MAP_NOEXEC);
    return true;
  }

  return process.getLoader().load(addr, process.getAddrSpace());
}
