// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <lib/shared_ptr.h>
#include <lib/unique_ptr.h>
#include <lib/vector.h>
#include <sync/mutex.h>
#include <addr_space.h>
#include <lib/map.h>
#include <fs/fd.h>

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

    UserProcess(lib::shared_ptr<Loader> loader,
                lib::shared_ptr<Terminal> term);
    explicit UserProcess(const UserProcess& forkee);
    ~UserProcess() = default;

    inline ProcState state() const { return m_state; }
    inline size_t pid() const { return m_pid; }
    size_t threadCount() const { return m_taskList.size(); } // TODO: don't count killed threads
    bool isOwnProcess() const;
    AddrSpace& getAddrSpace() { return *m_addrSpace; }
    Terminal& getTerm() { return *m_term; }
    const Loader& getLoader() { return *m_loader; }
    bool isValidStackAddr(size_t addr) const;

    size_t addTask(void* entryPoint, void* arg1 = nullptr, void* arg2 = nullptr);
    ssize_t fork();
    void exit(int status);
    void checkForDeadTasks();

    bool closeFile(size_t fd);
    int insertOpenFile(const fs::FileDesc& fd, ssize_t fdNum = -1);
    fs::FileDesc getOpenFile(size_t fd);

  protected: /* UserTask can access  */
    UserStack allocStack();
    void releaseStack(UserStack stack);

  private:
    static size_t getNewPid();
    void killAllTasks();

    ProcState m_state;
    size_t m_pid;

    lib::unique_ptr<AddrSpace> m_addrSpace;
    lib::shared_ptr<Loader> m_loader;
    lib::shared_ptr<Terminal> m_term;

    Mutex m_taskListLock;
    lib::List<UserTask*> m_taskList;

    mutable Mutex m_stackListLock;
    lib::vector<bool> m_stackList;

    Mutex m_filesLock;
    lib::Map<size_t, fs::FileDesc> m_files;
};
