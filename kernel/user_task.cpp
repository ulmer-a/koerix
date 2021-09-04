#include <user_task.h>
#include <addr_space.h>
#include <context.h>
#include <debug.h>
#include <user_proc.h>
#include <loader.h>

UserTask::UserTask(UserProcess& process)
  : Task(process.getAddrSpace())
  , m_process(process)
{
}

void UserTask::initContext()
{
  /* User stack pointer: subtract a pointer size to
   * make it unambigously identifyable as a stack address */
  auto userStackPtr = IDENT_OFFSET - sizeof(size_t);
  auto entryPoint = m_process.getLoader().getEntryPoint();

  auto ctx = context();
  initArchContext(ctx,
    entryPoint,     // entry point
    userStackPtr,   // stack pointer
    0,              // first argument
    0               // second argument
  );

  debug() << "created new user task with TID " << tid() << "\n";
}
