// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <canary.h>

class Lock : public ktl::Canary
{
  public:
    Lock() = default;
    virtual ~Lock() = default;

    virtual void lock() = 0;
    virtual void unlock() = 0;
};

class ScopedLock
{
  public:
    ScopedLock(Lock& lock)
      : m_lock(lock)
    {
      m_lock.lock();
    }

    ~ScopedLock() {
      m_lock.unlock();
    }

  private:
    Lock& m_lock;
};
