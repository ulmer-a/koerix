// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>
#include <mutex.h>
#include <list.h>
#include <sync/irq_lock.h>

class IQueue
{
  public:
    static const size_t QUEUE_BUFFER_SIZE = 4096;

    IQueue(Lock& bufferLock);
    virtual ~IQueue();

    ssize_t read(char *buffer, size_t len);
    ssize_t write(char* buffer, size_t len);

  private:
    Lock& m_bufferLock;
    char m_buffer[QUEUE_BUFFER_SIZE];

    size_t m_available;
    size_t m_readPtr, m_writePtr;

    size_t m_subscriberCount;
    ktl::List<Task*> m_readSubscribers;
    void wakeupSubscribers();
};

class Queue : public IQueue
{
  public:
    Queue();

  private:
    Mutex m_bufferLock;
};

class IrqContextQueue : public IQueue
{
  public:
    IrqContextQueue();

  private:
    sync::IrqDisableLock m_bufferLock;
};
