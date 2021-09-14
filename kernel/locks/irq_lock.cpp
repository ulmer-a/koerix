#include <sync/irq_lock.h>
#include <arch/asm.h>

sync::IrqDisableLock::~IrqDisableLock()
{
  assert(m_locked == false);
}

void sync::IrqDisableLock::lock()
{
  verify();

  m_irqsWereEnabled = irqEnabled();
  if (m_irqsWereEnabled)
    cli();

  m_locked = true;
}

void sync::IrqDisableLock::unlock()
{
  verify();
  assert(m_locked);

  m_locked = false;

  if (m_irqsWereEnabled)
    sti();
}
