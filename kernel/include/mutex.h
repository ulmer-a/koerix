// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <list.h>
#include <spin.h>
#include <lock.h>

class Task;

class Mutex : public Lock
{
  public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();

    /* this function might yield internally */
    Task* getHolder();

    bool isHeld();

  private:
    size_t m_lock;
    Task* m_heldBy;

    /* a lists of tasks waiting for this mutex
     * to be released. */
    ktl::List<Task*> m_waitingTasks;
    Spinlock m_waitingTasksLock;
};
