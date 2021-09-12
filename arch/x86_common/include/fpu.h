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
