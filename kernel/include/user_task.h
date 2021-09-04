#pragma once

#include <task.h>

class Loader;
class UserProcess;

class UserTask : public Task
{
  public:
    UserTask(UserProcess& process);
    ~UserTask() = default;

    void initContext();

    bool isUserTask() const final { return true; }

  private:
    UserProcess& m_process;
};
