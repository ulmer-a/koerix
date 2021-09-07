#include <mutex.h>
#include <arch/asm.h>
#include <task.h>
#include <scheduler.h>

Mutex::Mutex()
  : m_lock(0)
  , m_heldBy(nullptr)
{ }

Mutex::~Mutex()
{
  verify();
  assert(m_lock == 0);
  assert(m_heldBy == nullptr);
  assert(m_waitingTasks.size() == 0);
}

void Mutex::lock()
{
  verify();

  auto currentTask = sched::currentTask();
  while (xchg(1, &m_lock))
  {
    assert(currentTask != nullptr);

    /* the mutex is locked, so we're going to put
     * ourselves onto the list of waiting tasks. */
    m_waitingTasksLock.lock();
    m_waitingTasks.push_back(currentTask);

    /* since we still hold the waitingTasks lock at
     * this point, nobody will try to wake us up until
     * we actually have gone to sleep.
     * now, we have to put ourselves to sleep as well as
     * unlock the waitingTasks spinlock without a
     * reschedule in between. */
    sched::disable();
    currentTask->sleep();
    m_waitingTasksLock.unlock();
    sched::enable();
  }

  m_heldBy = currentTask;
}

void Mutex::unlock()
{
  verify();

  /* we can only unlock a mutex if it's actually
   * aquired by us (the current task) */
  assert(m_lock == 1);
  assert(m_heldBy == sched::currentTask());

  /* unlock */
  m_heldBy = nullptr;
  m_lock = 0;

  /* wakeup some task from the list of waiting
   * tasks so that they can go for the lock again. */
  m_waitingTasksLock.lock();
  if (m_waitingTasks.size() > 0)
  {
    auto task = m_waitingTasks.pop_front();
    task->resume();
  }
  m_waitingTasksLock.unlock();
}

Task* Mutex::getHolder()
{
  verify();

  while (m_lock)
  {
    if (m_heldBy == nullptr)
    {
      sched::yield();
      continue;
    }
    return m_heldBy;
  }
  return nullptr;
}

bool Mutex::isHeld()
{
  return (sched::currentTask() == getHolder());
}
