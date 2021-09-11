#pragma once

#include <x86/features.h>

struct FpuContextStruct;

class FpuContext
{
  public:
    FpuContext();
    ~FpuContext();

    FpuContext* clone();

    void save();
    void restore() const;

  private:
    void* m_data;
    FpuContextStruct* m_context;
};

void fpuFault();
void fpuClear();
