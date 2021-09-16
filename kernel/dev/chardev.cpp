// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <dev/chardev.h>
#include <arch/asm.h>
#include <lib/errno.h>

namespace devices {

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
