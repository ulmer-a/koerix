#pragma once

#include <mutex.h>
#include <list.h>

class UserProcess;

class ProcList
{
    friend class UserProcess;

  public:
    static ProcList& get();

    size_t procCount();
    void checkForDeadProcesses();

  protected:
    void onAddProcess(UserProcess* proc);

  private:
    ProcList() = default;
    static ProcList* s_instance;

    Mutex m_procListLock;
    ktl::List<UserProcess*> m_procList;
};
