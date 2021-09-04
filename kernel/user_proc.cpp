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

void UserProcess::addTask(UserTask* task)
{
  m_taskListLock.lock();
  m_taskList.push_back(task);
  m_taskListLock.unlock();
}
