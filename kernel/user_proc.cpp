#include <user_proc.h>
#include <loader.h>
#include <scheduler.h>
#include <user_task.h>
#include <proc_list.h>
#include <scheduler.h>

UserProcess::UserProcess(ktl::shared_ptr<Loader> loader,
                         ktl::shared_ptr<Terminal> term)
  : m_state(RUNNING)
  , m_addrSpace(new AddrSpace())
  , m_loader(loader)
  , m_term(term)
{
  /* add this process to the global list of processes */
  ProcList::get().onAddProcess(this);

  assert(m_loader->isValid());

  /* create a main thread for this process, the entry point
   * is defined by the entry address found in the ELF binary. */
  addTask(m_loader->entryAddr());
}

bool UserProcess::isOwnProcess() const
{
  auto currentTask = sched::currentTask();
  if (!currentTask->isUserTask())
    return false;
  auto userTask = (UserTask*)currentTask;
  return (&userTask->getProcess() == this);
}

size_t UserProcess::addTask(void* entryPoint)
{
  ScopedMutex smtx { m_taskListLock };
  assert(m_state == RUNNING);
  auto task = new UserTask(*this, entryPoint);
  m_taskList.push_back(task);

  /* insert the task into the scheduler's run
   * queue. this will effectively start the task.
   * do this *after* it has been inserted
   * into the task list of this process. */
  sched::insertTask(task);
  return task->tid();
}

void UserProcess::exit(int status)
{
  /* acquire the task list lock, so that no new tasks can be
   * added. this will also prevent the process from being
   * killed multiple times. */
  m_taskListLock.lock();
  if (m_state != RUNNING)
  {
    m_taskListLock.unlock();

    /* if the calling task is a task of a process that has
     * already been killed, then kill ourselves. otherwise,
     * we're done as the process has already been killed. */
    if (isOwnProcess())
      sched::currentTask()->exit();
    return;
  }

  /* a process in the killed state will remain allocated as long
   * as there are tasks around. the system will occasionally call
   * checkForDeadTasks() which removes any ceased tasks. Then, and
   * only then, the process can safely be deleted. */
  m_state = KILLED;

  /* kill all remaining threads. this doesn't take immediate effect,
   * because a task can only be killed by itself, so when we send a
   * kill request to a task we must wait until they have done so. */
  killAllTasks();

  m_taskListLock.unlock();
  if (isOwnProcess())
    sched::currentTask()->exit();
}

void UserProcess::checkForDeadTasks()
{
  ScopedMutex smtx { m_taskListLock };
  for (auto it = m_taskList.begin(); it != nullptr; it = it->next)
  {
    auto userTask = it->item;
    if (userTask->state() == Task::KILLED)
    {
      /* don't delete ourselves while we're running */
      assert(userTask != sched::currentTask());

      delete userTask;
      debug() << "tid " << userTask->tid() << ": deleted\n";

      /* some boilerplate code to remove the task from the list and
       * not break the for loop at the same time:  */
      auto prev = it->prev;
      m_taskList.remove(it);
      it = prev;
      if (it == nullptr) {
        it = m_taskList.begin();
        if (it == nullptr)
          break;
      }
    }
  }

  /* after there's no more tasks in the list, the process
   * can safely be marked ready for deletion. */
  if (m_taskList.size() == 0)
    m_state = TO_BE_DELETED;
}

void UserProcess::killAllTasks()
{
  assert(m_taskListLock.isHeld());

  for (auto it = m_taskList.begin(); it != nullptr; it = it->next)
  {
    if (it->item == sched::currentTask())
    {
      /* don't kill ourselves! */
      continue;
    }

    (*it)->asyncExit();
  }
}

bool UserProcess::isValidStackAddr(size_t addr) const
{
  ScopedMutex smtx { m_stackListLock };
  size_t i = (USER_BREAK - addr) / UserStack::getStackSize();
  if (m_stackList.size() <= i)
    return false;
  return m_stackList[i];
}

UserStack UserProcess::allocStack()
{
  size_t i;
  ScopedMutex smtx { m_stackListLock };
  for (i = 0; i < m_stackList.size(); i++)
  {
    // check if the stack is already allocated
    if (m_stackList[i] == true)
      continue;

    return UserStack(USER_BREAK - (UserStack::getStackSize() * i));
  }

  /* if all stacks in the vector are allocted, create a
   * new one and allocate it */
  m_stackList.push_back(true);
  return UserStack(USER_BREAK - (UserStack::getStackSize() * i));
}

void UserProcess::releaseStack(UserStack stack)
{
  ScopedMutex smtx { m_stackListLock };
  size_t i = (USER_BREAK - stack.getStackPtr()) / UserStack::getStackSize();
  assert(m_stackList.size() > i);
  assert(m_stackList[i] == true);

  if (m_stackList.size() == i + 1)
    m_stackList.pop_back();
  else
    m_stackList[i] = false;
}
