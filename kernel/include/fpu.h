// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <arch/fpu.h>

namespace fpu {

  struct ContextStruct;

  class Context
  {
    public:
      Context();

      Context* clone();

      void save();
      void restore() const;

      static void clear();

    private:
      ContextImpl m_impl;
  };

  void onFault();

  extern "C" void fpuEnableTrap();
  extern "C" void fpuClearTrap();
}
