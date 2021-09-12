// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>

class UserStack
{
  public:
    explicit UserStack(size_t topAddr)
      : m_top(topAddr) { }

    inline size_t getStackPtr() const {
      return m_top;
    }

    static inline size_t getStackSize() {
      return 8*1024*1024;
    }

    inline size_t getStackBase() const {
      return m_top - getStackSize();
    }

  private:
    size_t m_top;
};
