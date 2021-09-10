#pragma once

#include <shared_ptr.h>
#include <unique_ptr.h>
#include <vector.h>
#include <mutex.h>
#include <addr_space.h>

class Loader;
class UserTask;
class UserStack;
class Terminal;

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

    UserProcess(ktl::shared_ptr<Loader> loader,
                ktl::shared_ptr<Terminal> term);
    ~UserProcess() = default;

    inline ProcState state() const { return m_state; }
    size_t threadCount() const { return m_taskList.size(); } // TODO: don't count killed threads
    bool isOwnProcess() const;
    AddrSpace& getAddrSpace() { return *m_addrSpace; }
    Terminal& getTerm() { return *m_term; }
    const Loader& getLoader() { return *m_loader; }
    bool isValidStackAddr(size_t addr) const;

    size_t addTask(void* entryPoint, void* arg1 = nullptr, void* arg2 = nullptr);
    void exit(int status);
    void checkForDeadTasks();

  protected: /* UserTask can access  */
    UserStack allocStack();
    void releaseStack(UserStack stack);

  private:
    void killAllTasks();

    ProcState m_state;

    ktl::unique_ptr<AddrSpace> m_addrSpace;
    ktl::shared_ptr<Loader> m_loader;
    ktl::shared_ptr<Terminal> m_term;

    Mutex m_taskListLock;
    ktl::List<UserTask*> m_taskList;

    mutable Mutex m_stackListLock;
    ktl::vector<bool> m_stackList;
};
