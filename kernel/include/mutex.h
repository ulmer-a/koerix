#pragma once

#include <canary.h>
#include <list.h>
#include <spin.h>

class Task;

class Mutex : public ktl::Canary
{
  public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();

    /* this function might yield internally */
    Task* getHolder();

  private:
    size_t m_lock;
    Task* m_heldBy;

    /* a lists of tasks waiting for this mutex
     * to be released. */
    ktl::List<Task*> m_waitingTasks;
    Spinlock m_waitingTasksLock;
};

class ScopedMutex
{
  public:
    ScopedMutex(Mutex& mtx)
      : m_mutex(mtx)
    {
      m_mutex.lock();
    }

    ~ScopedMutex() {
      m_mutex.unlock();
    }

  private:
    Mutex& m_mutex;
};
