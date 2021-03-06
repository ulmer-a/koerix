// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

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
    debug(PAGEFAULT) << "PF: nullptr dereferenced!\n";
    return false;
  }

  if (addr >= USER_BREAK) {
    debug(PAGEFAULT) << "PF: referencing (unmapped) kernel memory!\n";
    return false;
  }

  return true;
}

void killProgram()
{
  auto& proc = sched::currentUserTask()->getProcess();
  debug(PAGEFAULT) << "PID " << proc.pid()
          << ": KILLED due to invalid memory reference\n";
  proc.exit(-1);
}

bool handlePageFault(size_t addr, FaultFlags flags)
{
  assert((flags & PF_RESERVED) == 0);

  if (addr == EXIT_ADDR && (flags & PF_CODE))
  {
    /* the thread has been sent a kill request (because the
     * process exited or whatever). So cleanly exit the thread. */
    sched::currentTask()->exit();
  }

  if (!checkIfValid(addr, flags))
  {
    killProgram();
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

  if (!process.getLoader().load(addr, process.getAddrSpace()))
  {
    killProgram();
    return false;
  }

  return true;
}
