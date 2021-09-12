// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

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
    UserTask(const UserTask& self) = delete;
    UserTask(UserProcess& process, const UserTask& forkee);
    ~UserTask();

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
