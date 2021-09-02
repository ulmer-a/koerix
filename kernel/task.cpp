#include <task.h>
#include <mm.h>
#include <string.h>
#include <context.h>
#include <asm.h>
#include <debug.h>
#include <scheduler.h>

#define KERNEL_STACK_SIZE 8192

Task::Task(AddrSpace& vspace)
  : m_tid(getNewTid())
  , m_state(RUNNING)
  , m_addrSpace(vspace)
{
  /* allocate a kernel stack and make the context struct
   * point to a location on the top. the derived class is
   * responsible for filling in the struct with data by
   * writing the fields in m_context. */
  m_kernelStackBase = (uint8_t*)kmalloc(KERNEL_STACK_SIZE);
  m_kernelStackTop = m_kernelStackBase + KERNEL_STACK_SIZE;
  m_context = (IrqContext*)m_kernelStackTop - 1;
  memset((void*)m_context, 0, sizeof(IrqContext));
}

Task::~Task()
{
  /* release the kernel stack memory */
  kfree(m_kernelStackBase);
}

void Task::exit()
{
  assert(sched::currentTask() == this);
  m_state = KILLED;
  sched::yield();
}

bool Task::schedulable()
{
  /* this function is called by the schedule()
   * and therefore is running in an interrupt
   * context with further interrupts disabled.
   * don't use any locks and don't block */

  return (m_state == RUNNING);
}

size_t Task::getNewTid()
{
  static size_t tid_counter = 1;
  return atomic_add(&tid_counter, 1);
}
