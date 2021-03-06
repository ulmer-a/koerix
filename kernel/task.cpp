// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <task.h>
#include <mm.h>
#include <lib/string.h>
#include <context.h>
#include <sync/atomic.h>
#include <debug.h>
#include <scheduler.h>
#include <sync/lock.h>

#define KERNEL_STACK_SIZE 8192

extern uint64_t s_timerTicks;

namespace sched {
  extern bool s_schedEnable;
}

Task::Task(AddrSpace& vspace)
  : m_tid(getNewTid())
  , m_state(RUNNING)
  , m_addrSpace(vspace)
  , m_wakeUpAt((size_t)-1)
{
  /* allocate a kernel stack and make the context struct
   * point to a location on the top. the derived class is
   * responsible for filling in the struct with data by
   * writing the fields in m_context. */
  m_kernelStack = kmalloc(KERNEL_STACK_SIZE);
  m_kernelStackTop = (size_t)m_kernelStack.get() + KERNEL_STACK_SIZE;
  m_context = (IrqContext*)m_kernelStackTop - 1;
  new (m_context) IrqContext();
}

void Task::exit()
{
  debug(TASK) << "tid " << tid() << ": terminating\n";
  assert(sched::currentTask() == this);
  m_state = KILLED;
  sched::yield();
}

void Task::sleep()
{
  assert(sched::currentTask() == this);
  assert(m_state == RUNNING);
  m_state = SLEEPING;

  if (sched::isEnabled())
    sched::yield();
}

void Task::sleepAndUnlock(Lock* lock)
{
  sched::s_schedEnable = false;
  sleep();
  lock->unlock();
  sched::s_schedEnable = true;

  /* since we disbled the scheduler before, we might not
   * actually be sleeping yet, so enforce it now. */
  sched::yield();
}

void Task::sleepTicks(size_t timerTicks)
{
  m_wakeUpAt = s_timerTicks + timerTicks;
  sleep();
}

void Task::resume()
{
  assert(m_state == SLEEPING);
  assert(sched::currentTask() != this);
  m_state = RUNNING;
}

bool Task::schedulable()
{
  /* this function is called by the schedule()
   * and therefore is running in an interrupt
   * context with further interrupts disabled.
   * don't use any locks and don't block */

  if (m_state == SLEEPING && s_timerTicks > m_wakeUpAt)
  {
    m_state = RUNNING;
    m_wakeUpAt = (size_t)-1;
  }

  return (m_state == RUNNING);
}

size_t Task::getNewTid()
{
  static size_t tid_counter = 1;
  return atomic_add(&tid_counter, 1);
}
