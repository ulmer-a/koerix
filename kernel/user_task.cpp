#include <user_task.h>
#include <addr_space.h>
#include <context.h>
#include <debug.h>
#include <user_proc.h>
#include <loader.h>
#include <scheduler.h>

UserTask::UserTask(UserProcess& process)
  : Task(process.getAddrSpace())
  , m_process(process)
{
  initContext(m_process.getLoader().getEntryPoint());
  sched::insertTask(this);
}

UserTask::UserTask(UserProcess& process, void* entry)
  : Task(process.getAddrSpace())
  , m_process(process)
{
  initContext((size_t)entry);
}

void UserTask::initContext(size_t entryPoint)
{
  /* User stack pointer: subtract a pointer size to
   * make it unambigously identifyable as a stack address */
  auto userStackPtr = IDENT_OFFSET - sizeof(size_t);

  auto ctx = context();
  initArchContext(ctx, true,
    entryPoint,     // entry point
    userStackPtr,   // stack pointer
    0,              // first argument
    0               // second argument
  );

  debug() << "created new user task with TID " << tid() << "\n";
}
