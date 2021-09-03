#pragma once

#include <canary.h>

class Spinlock : public ktl::Canary
{
  public:
    Spinlock();
    ~Spinlock();

    void lock();
    void unlock();

  private:
    size_t m_lock;
};
