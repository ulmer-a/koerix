#include <fs/dir.h>
#include <errno.h>


bool fs::Dir::isMountPoint() const
{
  return !m_mountOverlay.isNull();
}

bool fs::Dir::mount(ktl::shared_ptr<fs::Dir> fs, int& error)
{
  ScopedMutex smtx { m_mountLock };
  if (this->isMountPoint())
  {
    error = ENOTSUP;
    return false;
  }

  m_mountOverlay = fs;
  return true;
}

fs::File* fs::Dir::lookup(const char* path, int& error)
{
  ScopedMutex smtx { m_mountLock };
  if (!m_mountOverlay.isNull())
    return nullptr;//doLookup(path, error);
  return m_mountOverlay->lookup(path, error);
}

ssize_t fs::Dir::read(char* buf, size_t len)
{
  return -ENOTSUP;
}

ssize_t fs::Dir::write(char* buf, size_t len)
{
  return -ENOTSUP;
}
