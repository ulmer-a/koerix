#pragma once

#include <shared_ptr.h>
#include <unique_ptr.h>
#include <vector.h>
#include <mutex.h>

class Loader;
class AddrSpace;
class UserTask;
class UserStack;

class UserProcess
{
    friend class UserTask;

  public:
    enum ProcState
    {
      RUNNING,
      KILLED
    };

    UserProcess(ktl::shared_ptr<Loader>& loader);
    ~UserProcess() = default;

    inline AddrSpace& getAddrSpace() {
      return *m_addrSpace;
    }

    inline const Loader& getLoader() {
      return *m_loader;
    }

    bool isOwnProcess() const;
    bool isValidStackAddr(size_t addr) const;
    void exit(int status);

    void checkForDeadTasks();

  protected: /* UserTask can access  */
    UserStack allocStack();
    void releaseStack(UserStack stack);

  private:
    void addTask();
    void killAllTasks();

  private:
    ProcState m_state;

    ktl::unique_ptr<AddrSpace> m_addrSpace;
    ktl::shared_ptr<Loader> m_loader;

    Mutex m_taskListLock;
    ktl::List<UserTask*> m_taskList;

    mutable Mutex m_stackListLock;
    ktl::vector<bool> m_stackList;
};
