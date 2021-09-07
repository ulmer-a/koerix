#include <fs/dir.h>
#include <errno.h>
#include <string.h>


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

ktl::shared_ptr<fs::File> fs::Dir::lookup(const char* path, int& error)
{
  ScopedMutex smtx { m_mountLock };
  if (m_mountOverlay.isNull())
    return doLookup(path, error);
  return m_mountOverlay->lookup(path, error);
}

ktl::shared_ptr<fs::File> fs::Dir::doLookup(const char* path, int& error)
{
  ScopedMutex smtx { m_fileLock };
  for (size_t i = 0; i < m_files.size(); i++)
  {
    if (matchesFileName(m_files[i]->name, path))
      return m_files[i]->file;
  }
  error = ENOENT;
  return ktl::shared_ptr<fs::File>();
}

bool fs::Dir::matchesFileName(const char* path, const char* reference)
{
  while (*path && *reference && *path != '/')
  {
    if (*path++ != *reference++)
      return false;
  }
  return true;
}

ssize_t fs::Dir::read(char* buf, size_t len)
{
  return -ENOTSUP;
}

ssize_t fs::Dir::write(char* buf, size_t len)
{
  return -ENOTSUP;
}

void fs::Dir::addFile(const char* name, ktl::shared_ptr<fs::File> file)
{
  ScopedMutex smtx { m_fileLock };
  auto entry = new Direntry;
  strcpy(entry->name, name);
  entry->file = file;
  m_files.push_back(entry);
}
