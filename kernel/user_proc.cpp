#include <user_proc.h>
#include <addr_space.h>
#include <loader.h>

UserProcess::UserProcess(ktl::shared_ptr<Loader>& loader)
  : m_addrSpace(new AddrSpace())
  , m_loader(loader)
{
  assert(m_loader->isValidBinary());
}
