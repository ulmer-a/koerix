#pragma once

#include <types.h>
#include <list.h>

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
    virtual ~Task();

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

    void exit();
    bool schedulable();
    virtual bool isUserTask() const = 0;

  private:
    static size_t getNewTid();

    size_t m_tid;
    TaskState m_state;
    AddrSpace& m_addrSpace;
    uint8_t* m_kernelStackBase;
    uint8_t* m_kernelStackTop;
    IrqContext* m_context;
};
