// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>
#include <dev/devfs.h>
#include <fifo.h>

namespace devices {

  class CharDev : public dev::DeviceFile
  {
    public:
      CharDev(size_t major, size_t minor);

      FileType getType() const final { return CHARDEV; }

    protected:
      static size_t getNewMajor();
  };

}
