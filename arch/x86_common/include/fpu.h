#pragma once

#include <features.h>

struct FpuContextStruct;

class FpuContext
{
  public:
    FpuContext();
    ~FpuContext();

    void save();
    void restore() const;

  private:
    void* m_data;
    FpuContextStruct* m_context;
};

void fpuFault();
void fpuClear();
