#include <fs/fd.h>
#include <fs/file.h>

using namespace fs;

FileDesc::FileDesc(File& file)
  : m_file(file)
{

}

ssize_t FileDesc::read(char* buf, size_t len)
{
  return m_file.read(buf, len);
}

ssize_t FileDesc::write(char* buf, size_t len)
{
  return m_file.write(buf, len);
}
