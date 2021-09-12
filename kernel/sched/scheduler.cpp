#include <scheduler.h>
#include <arch/asm.h>
#include <list.h>
#include <task.h>
#include <context.h>
#include <mm.h>
#include <kernel_task.h>
#include <addr_space.h>
#include <user_task.h>
#include <proc_list.h>
#include <fpu.h>

/* sets the stack pointer that will be used
 * when the thread returns to kernel code */
extern "C" void setKernelStackPtr(size_t stackptr);

namespace sched {

  /* the scheduler picks it's tasks from s_taskList. It's important
   * that the list is not modified when the scheduler runs. only the
   * helpers may modify it when the scheduler is disabled.  */
  ktl::List<Task*> s_taskList;

  bool s_schedEnable = false;
  Task* s_currentTask;

  void yield()
  {
    if (s_schedEnable)
      arch_yield();
  }

  static void idle_task(void* arg)
  {
    for (;;)
    {
      /* when the system is on idle, do some cleanup work */
      ProcList::get().checkForDeadProcesses();

      hlt();
    }
  }

  void setup(Task* initialTask)
  {
    assert(!s_schedEnable);

    new (&s_taskList) ktl::List<Task*>();
    s_taskList.push_back(initialTask);
    s_taskList.push_back(new KernelTask(idle_task));
    s_schedEnable = true;
  }

  void insertTask(Task *task)
  {
    assert(s_schedEnable);
    assert(task->state() == Task::RUNNING);

    /* pre-allocate the memory required by push_back()
     * so that we don't call 'new' when the scheduler is
     * not running (might yield a deadlock)  */
    void* mem = kmalloc(sizeof(ktl::ListItem<Task*>));

    s_schedEnable = false;
    s_taskList.push_back(task, mem);
    s_schedEnable = true;
  }

  void removeTask(Task* task)
  {
    assert(s_schedEnable);
    assert(task->state() == Task::KILLED);

    auto it = s_taskList.find(task);
    assert(it != nullptr);

    s_schedEnable = false;
    s_taskList.remove_it(it);
    s_schedEnable = true;

    /* delete the linked list element only after scheduling
     * has been enabled, again. this might otherwise lead to
     * a deadlock. */
    delete it;
  }

  bool isEnabled()
  {
    return s_schedEnable;
  }

  Task* currentTask()
  {
    return s_currentTask;
  }

  UserTask* currentUserTask()
  {
    assert(s_currentTask->isUserTask());
    return (UserTask*)s_currentTask;
  }

  namespace core {

    Task* getNextTask()
    {
      Task* task;
      do {
        s_taskList.rotate();
        task = s_taskList.front();
        assert(task != nullptr);
      } while (!task->schedulable());
      return task;
    }

    IrqContext* schedule(IrqContext *ctx)
    {
      if (!s_schedEnable)
        return ctx;

      auto nextTask = getNextTask();
      if (nextTask != s_currentTask)
      {
        if (s_currentTask)
        {
          s_currentTask->setContext(ctx);
        }

        ctx = nextTask->context();

        if (!s_currentTask ||
            &s_currentTask->addrSpace() != &nextTask->addrSpace())
        {
          nextTask->addrSpace().apply();
        }

        resetFpuFlag();
        setKernelStackPtr(nextTask->kernelStackPtr());

        s_currentTask = nextTask;
      }

      return ctx;
    }
  }

  size_t taskCount()
  {
    return s_taskList.size();
  }
}
