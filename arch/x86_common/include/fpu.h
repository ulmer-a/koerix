// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <x86/features.h>

struct FpuContextStruct;

class FpuContext
{
  public:
    FpuContext();

    FpuContext* clone();

    void save();
    void restore() const;

  private:
    char m_data[512 + 16];
    FpuContextStruct* m_context;
};

void fpuFault();
void fpuClear();
