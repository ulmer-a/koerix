#include <fifo.h>
#include <debug.h>
#include <arch/asm.h>
#include <scheduler.h>
#include <task.h>
#include <errno.h>

Fifo::Fifo(size_t size)
  : m_data(new char[size])
  , m_available(0)
  , m_size(size)
  , m_readPtr(0)
  , m_writePtr(0)
{ }

Fifo::Fifo(char* buffer, size_t size)
  : m_data(buffer)
  , m_available(0)
  , m_size(size)
  , m_readPtr(0)
  , m_writePtr(0)
{ }

Fifo::~Fifo()
{

}

void Fifo::put(char c, bool lock)
{
  if (lock) cli();

  /* write a character, increment the write pointer
   * or let it wrap around.  */
  assert(m_writePtr < m_size);
  m_data[m_writePtr++] = c;
  if (m_writePtr == m_size)
    m_writePtr = 0;

  /* if the amount of available bytes is smaller than
   * the buffer size, just increment the amount of
   * available bytes. otherwise, increment the read
   * pointer so that get() will always return the m_size
   * newest characters.  */
  if (m_available < m_size)
  {
    m_available += 1;
  }
  else
  {
    m_readPtr += 1;
    if (m_readPtr == m_size)
      m_readPtr = 0;
  }

  if (lock) sti();
}

char Fifo::get()
{
  cli();
  if (m_available == 0)
  {
    sched::currentTask()->sleep();
  }

  /* read one byte from the buffer */
  assert(m_readPtr < m_size);
  char c = m_data[m_readPtr++] = c;
  if (m_readPtr == m_size)
    m_readPtr = 0;
  m_available -= 1;

  sti();
  return c;
}

ssize_t Fifo::read(char* buffer, size_t len)
{
  return -ENOTSUP;
}

ssize_t Fifo::write(char* buffer, size_t len)
{
  return -ENOTSUP;
}
