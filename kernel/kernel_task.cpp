#include <kernel_task.h>
#include <addr_space.h>
#include <context.h>
#include <debug.h>

static void ktask_runtime(KernelTask* task, void(*func)(void*))
{
  debug() << "kernel task #" << task->tid() << " started executing\n";
  func(task->arg());
  task->exit();
  debug() << "kernel task #" << task->tid() << " terminated\n";
}

KernelTask::KernelTask(void(*entry)(void* arg), void* arg)
  : Task(AddrSpace::kernel())
  , m_argument(arg)
{
  auto ctx = context();
  setInstructionPointer(ctx, (size_t)ktask_runtime);

  initKernelContext(ctx,
    (size_t)ktask_runtime,  // entry point
    kernelStackPtr(),       // stack pointer
    (size_t)this,           // first argument
    (size_t)entry           // second argument
  );

  debug() << "created new kernel task with TID " << tid() << "\n";
}