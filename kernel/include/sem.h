// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <canary.h>

class Semaphore : ktl::Canary
{
  public:
    Semaphore(size_t initialVal);
    ~Semaphore();

    void post();

  private:
    size_t m_value;
};
