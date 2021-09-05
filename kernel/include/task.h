#pragma once

#include <types.h>
#include <list.h>
#include <unique_ptr.h>

class AddrSpace;
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

    /* kernel stack, allocated on the heap */
    ktl::unique_ptr<uint8_t> m_kernelStack;
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

    inline IrqContext* context() {
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
