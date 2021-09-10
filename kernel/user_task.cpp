#include <user_task.h>
#include <addr_space.h>
#include <context.h>
#include <debug.h>
#include <user_proc.h>
#include <loader.h>
#include <scheduler.h>
#include <fpu.h>

UserTask::UserTask(UserProcess& process, void* entryPoint, void* arg1, void* arg2)
  : Task(process.getAddrSpace())
  , m_stack(process.allocStack())
  , m_process(process)
  , m_fpuContext(new FpuContext())
{
  context()->newUserCtx();
  context()->instructionPtr() = (size_t)entryPoint;
  context()->stackPtr() = m_stack.getStackPtr() - sizeof(void*);
  context()->arg1() = (size_t)arg1;
  context()->arg2() = (size_t)arg2;
  context()->arg3() = tid();
}

UserTask::UserTask(UserProcess& process, const UserTask& forkee)
  : Task(process.getAddrSpace())
  , m_stack(forkee.m_stack)
  , m_process(process)
  , m_fpuContext(forkee.m_fpuContext->clone())
{
  /* for fork(): copy the exact context of the task and set
   * the return value (for the child it must be 0) */
  *context() = *forkee.context();
  context()->returnValue() = 0;
}

UserTask::~UserTask()
{
  m_process.releaseStack(m_stack);
}

void UserTask::exit()
{
  fpuClear();
  Task::exit();
}

void UserTask::asyncExit()
{
  auto currentTask = sched::currentTask();
  assert(currentTask != this);

  /* we cannot just kill another thread, we have to
   * make the thread kill itself. so we modify the
   * thread's instruction pointer to generate an
   * exception when it's next scheduled. */
  context()->instructionPtr() = EXIT_ADDR;
}

void UserTask::setThreadPtr(void* ptr)
{
  m_threadPtr = ptr;
}

void* UserTask::getThreadPtr()
{
  return m_threadPtr;
}
