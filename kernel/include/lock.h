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
