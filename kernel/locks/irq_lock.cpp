#include <sync/irq_lock.h>
#include <arch/asm.h>

sync::IrqDisableLock::~IrqDisableLock()
{
  assert(m_locked == false);
}

void sync::IrqDisableLock::lock()
{
  cli();
  m_locked = true;
}

void sync::IrqDisableLock::unlock()
{
  m_locked = false;
  sti();
}
