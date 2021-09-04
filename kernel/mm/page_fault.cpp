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
  if ((flags & PF_USER) == 0 || (flags & PF_RESERVED))
    return false;

  if (!checkIfValid(addr, flags))
  {
    panic("PF: cannot recover yet from user page fault");
    // return false;
  }

  // 1. check if COW fault?
  // 2. check if stack fault?

  auto currentTask = sched::currentTask();
  assert(currentTask->isUserTask());
  auto& process = ((UserTask*)currentTask)->getProcess();

  return process.getLoader().load(addr, process.getAddrSpace());
}
