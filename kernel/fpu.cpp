#include <fpu.h>
#include <scheduler.h>
#include <user_task.h>
#include <arch/asm.h>

namespace fpu {

  static UserTask* s_fpuTask = nullptr;

  void onFault()
  {
    /* the current user task has accessed FPU/SSE/NEON registers or
     * instructions. for performance reasons we only save and
     * restore the FPU/SSE context on demand, which means that
     * the FPU/SSE context is not saved/restored on each individual
     * context switch. */

    fpuClearTrap();

    auto currentTask = sched::currentUserTask();
    if (s_fpuTask != currentTask)
    {
      if (s_fpuTask != nullptr)
        s_fpuTask->getFpuContext().save();
      currentTask->getFpuContext().restore();
      s_fpuTask = currentTask;
    }
  }

  void Context::clear()
  {
    cli();
    if (s_fpuTask == sched::currentTask())
    {
        s_fpuTask = nullptr;
        fpuEnableTrap();
    }
    sti();
  }

}
