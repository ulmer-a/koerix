#pragma once

#include <task.h>
#include <user_stack.h>

class Loader;
class UserProcess;
class UserStack;
class FpuContext;

class UserTask : public Task
{
  public:
    UserTask(UserProcess& process);
    ~UserTask();

    void initContext(size_t entryPoint);

    void exit() override;
    void asyncExit();

    bool isUserTask() const final {
      return true;
    }

    inline UserProcess& getProcess() const {
      return m_process;
    }

    inline FpuContext& getFpuContext() {
      return *m_fpuContext;
    }

  private:
    UserProcess& m_process;
    UserStack m_stack;

    ktl::unique_ptr<FpuContext> m_fpuContext;
};
