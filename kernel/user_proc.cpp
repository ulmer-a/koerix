#include <user_proc.h>
#include <loader.h>
#include <scheduler.h>
#include <user_task.h>
#include <proc_list.h>

UserProcess::UserProcess(ktl::shared_ptr<Loader> loader,
                         ktl::shared_ptr<Terminal> term)
  : m_state(RUNNING)
  , m_addrSpace(new AddrSpace())
  , m_loader(loader)
  , m_term(term)
{
  ProcList::get().onAddProcess(this);

  assert(m_loader->isValid());

  addTask();
}

bool UserProcess::isOwnProcess() const
{
  auto currentTask = sched::currentTask();
  if (!currentTask->isUserTask())
    return false;
  auto userTask = (UserTask*)currentTask;
  return (&userTask->getProcess() == this);
}

bool UserProcess::isValidStackAddr(size_t addr) const
{
  ScopedMutex smtx { m_stackListLock };
  size_t i = (USER_BREAK - addr) / UserStack::getStackSize();
  if (m_stackList.size() <= i)
    return false;
  return m_stackList[i];
}

void UserProcess::exit(int status)
{
  /* acquire the task list lock, so no new tasks can be
   * added. this will also prevent the process from being
   * killed multiple times. */
  m_taskListLock.lock();

  if (m_state != RUNNING)
  {
    m_taskListLock.unlock();

    if (isOwnProcess())
      sched::currentTask()->exit();
    return;
  }

  m_state = KILLED;
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
      assert(userTask != sched::currentTask());

      delete userTask;
      debug() << "tid " << userTask->tid() << ": deleted\n";

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

  if (m_taskList.size() == 0)
    m_state = TO_BE_DELETED;
}

void UserProcess::addTask()
{
  ScopedMutex smtx { m_taskListLock };
  auto task = new UserTask(*this);
  m_taskList.push_back(task);
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

UserProcess::ProcState UserProcess::state() const
{
  return m_state;
}

AddrSpace& UserProcess::getAddrSpace()
{
  return *m_addrSpace;
}

Terminal& UserProcess::getTerm()
{
  return *m_term;
}

const Loader& UserProcess::getLoader()
{
  return *m_loader;
}
