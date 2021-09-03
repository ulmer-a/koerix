#include <spin.h>
#include <arch/asm.h>
#include <scheduler.h>

Spinlock::Spinlock()
  : m_lock(0)
{ }

Spinlock::~Spinlock()
{
  verify();
  assert(m_lock == 0);
}

void Spinlock::lock()
{
  verify();

  while (xchg(1, &m_lock))
    sched::yield();
}

void Spinlock::unlock()
{
  verify();
  assert(m_lock == 1);
  m_lock = 0;
}
