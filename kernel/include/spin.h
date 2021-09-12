// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

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
