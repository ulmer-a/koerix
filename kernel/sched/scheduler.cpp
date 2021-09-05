#include <scheduler.h>
#include <arch/asm.h>
#include <list.h>
#include <task.h>
#include <context.h>
#include <mm.h>
#include <kernel_task.h>
#include <addr_space.h>
#include <features.h>
#include <user_task.h>

static bool s_schedEnable = false;
static ktl::List<Task*> s_taskList;
static Task* s_currentTask;

/* sets the stack pointer that will be used
 * when the thread returns to kernel code */
extern "C" void setKernelStackPtr(size_t stackptr);

static void idle_task(void* arg)
{
  debug() << "Welcome from the idle task!\n";
  for (;;) { hlt(); }
}

namespace sched {

  void yield()
  {
    if (s_schedEnable)
      arch_yield();
  }

  namespace init {
    void setup()
    {
      new (&s_taskList) ktl::List<Task*>();
      insertTask(new KernelTask(idle_task));
    }

    void insertTask(Task* task)
    {
      assert(!s_schedEnable);
      s_taskList.push_back(task);
    }
  }

  void enable() {
    s_schedEnable = true;
  }

  void disable() {
    s_schedEnable = false;
  }

  Task* getNextTask()
  {
    Task* task;
    do {
      s_taskList.rotate();
      task = s_taskList.front();
      assert(task != nullptr);

      if (task->state() == Task::KILLED)
      {
        /* if the task was killed, remove it from the
         * scheduler to cleanup the scheduler task list */
        s_taskList.remove(s_taskList.find(task));
      }

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

        if (s_currentTask->state() == Task::KILLED)
          s_taskList.remove(s_taskList.find(s_currentTask));
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

  void insertTask(Task *task)
  {
    assert(s_schedEnable);
    s_schedEnable = false;
    s_taskList.push_back(task);
    s_schedEnable = true;
  }

  Task* currentTask()
  {
    return s_currentTask;
  }

  bool isEnabled()
  {
    return s_schedEnable;
  }

  UserTask* currentUserTask()
  {
    assert(s_currentTask->isUserTask());
    return (UserTask*)s_currentTask;
  }

}
