// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <sync/mutex.h>
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
