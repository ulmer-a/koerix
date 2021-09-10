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
    UserTask(UserProcess& process, void* entryPoint,
             void* arg1 = nullptr, void* arg2 = nullptr);
    ~UserTask();

    void initContext(size_t entryPoint, size_t arg1, size_t arg2);

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

    void setThreadPtr(void* ptr);
    void* getThreadPtr();

  private:
    UserProcess& m_process;
    UserStack m_stack;

    void* m_threadPtr;
    ktl::unique_ptr<FpuContext> m_fpuContext;
};
