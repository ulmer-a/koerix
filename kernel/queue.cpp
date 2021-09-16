// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <queue.h>
#include <task.h>
#include <scheduler.h>
#include <sync/atomic.h>

IQueue::IQueue(Lock& bufferLock)
  : m_bufferLock(bufferLock)
  , m_available(0)
  , m_readPtr(0)
  , m_writePtr(0)
  , m_subscriberCount(0)
{ }

Queue::Queue()
  : IQueue(m_bufferLock)
{ }

IrqContextQueue::IrqContextQueue()
  : IQueue(m_bufferLock)
{ }

IQueue::~IQueue()
{
  // TODO: make this safe
  wakeupSubscribers();

  // TODO: replace this with a semaphore!!
  while (m_subscriberCount > 0)
    sched::yield();
}

ssize_t IQueue::read(char* buffer, size_t len)
{
  atomic_add(&m_subscriberCount, 1);
  if (m_available == 0)
  {
    void* mem = kmalloc(sizeof(lib::ListItem<Task*>));
    m_bufferLock.lock();
    m_readSubscribers.push_back(sched::currentTask(), mem);
    sched::currentTask()->sleepAndUnlock(&m_bufferLock);
  }

  ScopedLock smtx { m_bufferLock };
  size_t bytes_read = 0;
  while (m_available > 0 && bytes_read < len) {
    assert(m_readPtr < QUEUE_BUFFER_SIZE);
    *buffer++ = m_buffer[m_readPtr++];
    if (m_readPtr == QUEUE_BUFFER_SIZE)
      m_readPtr = 0;
    m_available -= 1;
    bytes_read += 1;
  }

  atomic_add(&m_subscriberCount, -1);
  assert(bytes_read <= len);
  return bytes_read;
}

ssize_t IQueue::write(char* buffer, size_t len)
{
  ScopedLock smtx { m_bufferLock };
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

void IQueue::wakeupSubscribers()
{
  while (m_readSubscribers.size() > 0)
    m_readSubscribers.pop_front()->resume();
}
