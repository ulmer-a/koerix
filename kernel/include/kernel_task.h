#pragma once

#include <task.h>

class KernelTask : public Task
{
  public:
    KernelTask(void(*entry)());
    ~KernelTask() = default;

    bool isUserTask() const { return false; }

  private:

};
