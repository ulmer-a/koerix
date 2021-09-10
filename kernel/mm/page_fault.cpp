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
    debug() << "PF: probably a nullpointer dereference\n";
    return false;
  }

  if (addr >= USER_BREAK) {
    debug() << "PF: user referencing kernel memory\n";
    return false;
  }

  return true;
}

bool handlePageFault(size_t addr, FaultFlags flags)
{
  assert((flags & PF_RESERVED) == 0);

  if ((flags & PF_USER) == 0)
  {
    debug() << "kernel got a page fault!!!\n";
    return false;
  }

  if (!checkIfValid(addr, flags))
  {
    return false;
  }

  auto currentTask = sched::currentTask();
  assert(currentTask->isUserTask());
  auto& process = ((UserTask*)currentTask)->getProcess();

  // 1. check if COW fault?
  // 2. check if stack fault?

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
