#pragma once

#include <shared_ptr.h>
#include <unique_ptr.h>
#include <vector.h>
#include <mutex.h>
#include <addr_space.h>

class Loader;
class UserTask;
class UserStack;

class UserProcess
{
    friend class UserTask;

  public:
    enum ProcState
    {
      RUNNING,
      KILLED,
      TO_BE_DELETED
    };

    UserProcess(ktl::shared_ptr<Loader>& loader);
    ~UserProcess() = default;

    ProcState state() const;
    bool isOwnProcess() const;
    AddrSpace& getAddrSpace();
    const Loader& getLoader();
    bool isValidStackAddr(size_t addr) const;
    void checkForDeadTasks();

    void exit(int status);

  protected: /* UserTask can access  */
    UserStack allocStack();
    void releaseStack(UserStack stack);

  private:
    void addTask();
    void killAllTasks();

    ProcState m_state;

    ktl::unique_ptr<AddrSpace> m_addrSpace;
    ktl::shared_ptr<Loader> m_loader;

    Mutex m_taskListLock;
    ktl::List<UserTask*> m_taskList;

    mutable Mutex m_stackListLock;
    ktl::vector<bool> m_stackList;
};
