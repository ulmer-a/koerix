#pragma once

#include <task.h>

class Loader;
class UserProcess;

class UserTask : public Task
{
  public:
    UserTask(UserProcess& process);
    UserTask(UserProcess& process, void* entry);
    ~UserTask() = default;

    void initContext(size_t entryPoint);

    bool isUserTask() const final { return true; }
    UserProcess& getProcess() const { return m_process; }

  private:
    UserProcess& m_process;
};