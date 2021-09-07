#include <fs/fd.h>
#include <fs/file.h>
#include <errno.h>

using namespace fs;


FileDesc::FileDesc()
  : m_valid(false)
  , m_write(false)
{

}

FileDesc::FileDesc(File* file, bool write)
  : m_valid(true)
  , m_file(file)
  , m_write(write)
{

}

FileDesc::~FileDesc()
{

}

ssize_t FileDesc::read(char* buf, size_t len)
{
  assert(m_file != nullptr);
  return m_file->read(buf, len);
}

ssize_t FileDesc::write(char* buf, size_t len)
{
  if (!m_write) {
    return -EPERM;
  }

  assert(m_file != nullptr);
  return m_file->write(buf, len);
}
