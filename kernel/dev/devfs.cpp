#include <dev/devfs.h>
#include <errno.h>

using namespace dev;

DeviceFile::DeviceFile(size_t major, size_t minor)
  : m_major(major)
  , m_minor(minor)
{

}

int DeviceFile::ioctrl(size_t cmd, size_t *arg)
{
  return -ENOTTY;
}

void DeviceFs::registerDevice(
    const char* name, ktl::shared_ptr<DeviceFile> device)
{

}
