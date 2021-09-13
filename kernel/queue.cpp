// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <queue.h>
#include <task.h>
#include <scheduler.h>
#include <arch/asm.h>

Queue::Queue()
  : m_available(0)
  , m_readPtr(0)
  , m_writePtr(0)
  , m_subscriberCount(0)
{ }

Queue::~Queue()
{
  wakeupSubscribers();

  // TODO: replace this with a semaphore!!
  while (m_subscriberCount > 0)
    sched::yield();
}

ssize_t Queue::read(char* buffer, size_t len)
{
  atomic_add(&m_subscriberCount, 1);
  if (m_available == 0)
  {
    m_subscriberLock.lock();
    m_readSubscribers.push_back(sched::currentTask());
    sched::currentTask()->sleepAndUnlock(&m_subscriberLock);
  }

  ScopedMutex smtx { m_bufferLock };
  size_t bytes_read = 0;
  while (m_available > 0) {
    assert(m_readPtr < QUEUE_BUFFER_SIZE);
    *buffer++ = m_buffer[m_readPtr++];
    if (m_readPtr == QUEUE_BUFFER_SIZE)
      m_readPtr = 0;
    m_available -= 1;
    bytes_read += 1;
  }

  atomic_add(&m_subscriberCount, -1);
  return bytes_read;
}

ssize_t Queue::write(char* buffer, size_t len)
{
  ScopedMutex smtx { m_bufferLock };
  for (size_t i = 0; i < len; i++)
  {
    /* write a character, increment the write pointer
     * or let it wrap around.  */
    assert(m_writePtr < QUEUE_BUFFER_SIZE);
    m_buffer[m_writePtr++] = buffer[i];
    if (m_writePtr == QUEUE_BUFFER_SIZE)
      m_writePtr = 0;

    /* if the amount of available bytes is smaller than
     * the buffer size, just increment the amount of
     * available bytes. otherwise, increment the read
     * pointer so that get() will always return the m_size
     * newest characters.  */
    if (m_available < QUEUE_BUFFER_SIZE)
    {
      m_available += 1;
    }
    else
    {
      m_readPtr += 1;
      if (m_readPtr == QUEUE_BUFFER_SIZE)
        m_readPtr = 0;
    }
  }

  wakeupSubscribers();
  return len;
}

void Queue::wakeupSubscribers()
{
  while (m_readSubscribers.size() > 0)
    m_readSubscribers.pop_front()->resume();
}
