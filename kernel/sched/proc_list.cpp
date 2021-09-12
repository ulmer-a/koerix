#include <proc_list.h>
#include <user_proc.h>

ProcList* ProcList::s_instance = nullptr;

ProcList& ProcList::get()
{
  if (s_instance == nullptr)
    s_instance = new ProcList();
  return *s_instance;
}

size_t ProcList::procCount()
{
  return m_procList.size();
}

void ProcList::checkForDeadProcesses()
{
  ScopedMutex smtx { m_procListLock };
  for (auto it = m_procList.begin(); it != nullptr; it = it->next)
  {
    auto proc = it->item;
    proc->checkForDeadTasks();

    if (proc->state() == UserProcess::TO_BE_DELETED)
    {
      delete proc;
      debug() << "proc: deleted\n";

      auto prev = it->prev;
      m_procList.remove(it);
      it = prev;
      if (it == nullptr) {
        it = m_procList.begin();
        if (it == nullptr)
          break;
      }
    }
  }
}

void ProcList::onAddProcess(UserProcess* proc)
{
  ScopedMutex smtx { m_procListLock };
  m_procList.push_back(proc);
}
