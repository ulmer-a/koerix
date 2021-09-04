#pragma once

#include <task.h>

class KernelTask : public Task
{
  public:
    KernelTask(void(*entry)(void*), void* arg = nullptr);
    ~KernelTask() = default;

    bool isUserTask() const final { return false; }
    void* arg() const { return m_argument; }

  private:
    void* m_argument;
};
