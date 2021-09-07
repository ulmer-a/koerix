#pragma once

#include <types.h>
#include <mutex.h>

class Fifo
{
  public:
    Fifo(size_t size);
    Fifo(char* buffer, size_t size);
    ~Fifo();

    ssize_t write(char* buffer, size_t len);
    ssize_t read(char *buffer, size_t len);

  private:
    Mutex m_subscriberLock;
    ktl::List<Task*> m_subscribers;
    void wakeupSubscribers();

    char* m_data;
    size_t m_available;
    const size_t m_size;

    size_t m_readPtr;
    size_t m_writePtr;
};

