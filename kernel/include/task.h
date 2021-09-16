// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>
#include <lib/list.h>
#include <lib/unique_ptr.h>

class AddrSpace;
class Lock;
struct IrqContext;

class Task
{
  public:
    enum TaskState
    {
      RUNNING,
      SLEEPING,
      KILLED
    };

    Task(AddrSpace& vspace);
    virtual ~Task() = default;

    /* exit will kill the task. this is possible only
     * from within the task itself. */
    virtual void exit();

    /* go to sleep. this can only be called from within
     * the task itself for now. */
    void sleep();
    void sleepAndUnlock(Lock* lock);
    void sleepTicks(size_t timerTicks);

    /* wake the task up from sleep. this can only be called
     * from outside the task. */
    void resume();

    /* called by the scheduler to query whether the task
     * is ready to be scheduled. */
    bool schedulable();

    virtual bool isUserTask() const = 0;

  private:
    static size_t getNewTid();

  private:
    size_t m_tid;
    TaskState m_state;
    AddrSpace& m_addrSpace;
    uint64_t m_wakeUpAt;

    /* kernel stack, allocated on the heap */
    lib::unique_ptr<uint8_t> m_kernelStack;
    size_t m_kernelStackTop;

    /* when the task is in an interrupt or system call,
     * this struct is populated with the suspended context. */
    IrqContext* m_context;

  public:
    /* Getters and setters that are called very often,
     * e.g. in the scheduler are inline, so they're fast */

    inline size_t tid() {
        return m_tid;
    }

    inline TaskState state() {
        return m_state;
    }

    inline IrqContext* context() const {
        return m_context;
    }

    inline void setContext(IrqContext* ctx) {
        m_context = ctx;
    }

    inline AddrSpace& addrSpace() {
      return m_addrSpace;
    }

    inline size_t kernelStackPtr() {
      return (size_t)m_kernelStackTop;
    }
};
