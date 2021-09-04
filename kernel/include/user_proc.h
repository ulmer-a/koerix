#pragma once

#include <shared_ptr.h>
#include <unique_ptr.h>
#include <mutex.h>

class Loader;
class AddrSpace;
class UserTask;

class UserProcess
{
  public:
    UserProcess(ktl::shared_ptr<Loader>& loader);
    ~UserProcess() = default;

    inline AddrSpace& getAddrSpace() {
      return *m_addrSpace;
    }

    inline const Loader& getLoader() {
      return *m_loader;
    }

  private:
    void addTask(UserTask* task);

  private:
    ktl::unique_ptr<AddrSpace> m_addrSpace;
    ktl::shared_ptr<Loader> m_loader;

    Mutex m_taskListLock;
    ktl::List<UserTask*> m_taskList;
};
