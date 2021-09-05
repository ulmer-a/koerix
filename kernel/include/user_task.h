#pragma once

#include <task.h>
#include <user_stack.h>

class Loader;
class UserProcess;
class UserStack;

class UserTask : public Task
{
  public:
    UserTask(UserProcess& process);
    ~UserTask() = default;

    void initContext(size_t entryPoint);

    bool isUserTask() const final { return true; }
    UserProcess& getProcess() const { return m_process; }

  private:
    UserProcess& m_process;
    UserStack m_stack;
};
