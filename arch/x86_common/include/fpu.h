// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

namespace fpu {

  struct ContextStruct;

  struct ContextImpl
  {
    char m_data[512 + 16];
    ContextStruct* m_context;
  };

}
