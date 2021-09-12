// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

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

ssize_t Fifo::write(char* buffer, size_t len)
{
  for (size_t i = 0; i < len; i++)
  {
    /* write a character, increment the write pointer
     * or let it wrap around.  */
    assert(m_writePtr < m_size);
    m_data[m_writePtr++] = buffer[i];
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
  }

  wakeupSubscribers();
  return len;
}

void Fifo::wakeupSubscribers()
{
  while (m_subscribers.size() > 0)
    m_subscribers.pop_front()->resume();
}

ssize_t Fifo::read(char* buffer, size_t len)
{
  cli();
  if (m_available == 0)
  {
    m_subscribers.push_back(sched::currentTask());
    sti();
    sched::currentTask()->sleep();
    cli();
  }

  size_t bytes_read = 0;
  while (m_available > 0) {
    assert(m_readPtr < m_size);
    *buffer++ = m_data[m_readPtr++];
    if (m_readPtr == m_size)
      m_readPtr = 0;
    m_available -= 1;
    bytes_read += 1;
  }
  sti();
  return bytes_read;
}
