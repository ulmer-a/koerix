#include <dev/chardev.h>
#include <arch/asm.h>
#include <errno.h>

namespace dev {

  CharDev::CharDev(size_t major, size_t minor)
    : DeviceFile(major, minor)
  {

  }

  size_t CharDev::getNewMajor()
  {
    static size_t majorCounter = 0;
    return atomic_add(&majorCounter, 1);
  }

}
