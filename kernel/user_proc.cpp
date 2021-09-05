#include <user_proc.h>
#include <addr_space.h>
#include <loader.h>
#include <user_task.h>

UserProcess::UserProcess(ktl::shared_ptr<Loader>& loader)
  : m_addrSpace(new AddrSpace())
  , m_loader(loader)
{
  assert(m_loader->isValidBinary());

  addTask(new UserTask(*this));
}

bool UserProcess::isValidStackAddr(size_t addr) const
{
  ScopedMutex smtx { m_stackListLock };
  size_t i = (USER_BREAK - addr) / UserStack::getStackSize();
  if (m_stackList.size() <= i)
    return false;
  return m_stackList[i];
}

void UserProcess::addTask(UserTask* task)
{
  ScopedMutex smtx { m_taskListLock };
  m_taskList.push_back(task);
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

  if (m_stackList.size() - 1 == i)
    m_stackList.pop_back();
  else
    m_stackList[i] = false;
}
