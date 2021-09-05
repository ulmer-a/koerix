#include <user_task.h>
#include <addr_space.h>
#include <context.h>
#include <debug.h>
#include <user_proc.h>
#include <loader.h>
#include <scheduler.h>
#include <fpu.h>

UserTask::UserTask(UserProcess& process)
  : Task(process.getAddrSpace())
  , m_stack(process.allocStack())
  , m_process(process)
  , m_fpuContext(new FpuContext())
{
  initContext(m_process.getLoader().getEntryPoint());
  sched::insertTask(this);
}

UserTask::~UserTask()
{
  m_process.releaseStack(m_stack);
}

void UserTask::initContext(size_t entryPoint)
{
  /* User stack pointer: subtract a pointer size to
   * make it unambigously identifyable as a stack address */
  auto userStackPtr = m_stack.getStackPtr() - sizeof(void*);

  auto ctx = context();
  initArchContext(ctx, true,
    entryPoint,     // entry point
    userStackPtr,   // stack pointer
    0,              // first argument
    0               // second argument
  );

  debug() << "created new user task with TID " << tid() << "\n";
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
  setInstructionPointer(context(), EXIT_ADDR);
}
