#pragma once

#include <types.h>
#include <dev/devfs.h>

namespace dev {

  class CharDev : public DeviceFile
  {
    public:
      CharDev(size_t major, size_t minor);

      FileType getType() const final { return CHARDEV; }

    protected:
      static size_t getNewMajor();
  };

}
