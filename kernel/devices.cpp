// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <dev/devices.h>
#include <platform.h>
#include <list.h>
#include <mm.h>
#include <string.h>
#include <dev/chardev.h>
#include <shared_ptr.h>
#include <dev/devfs.h>
#include <fs/vfs.h>

namespace dev {

  static ktl::shared_ptr<fs::Dir> s_devfs;

  void setup()
  {
    s_devfs = ktl::shared_ptr<fs::Dir>((fs::Dir*)new DeviceFs());

    // mount the device filesystem as rootfs for now
    int error;
    if (!fs::mount("/", s_devfs, error))
      assert(false);

    init_drivers();
  }

  DeviceFs& DeviceFs::get()
  {
    return *(DeviceFs*)s_devfs.get();
  }

  void registerDevice(const char* name, DeviceFile* dev)
  {
    debug(DEVICES) << "registered device '" << name << "'\n";
    DeviceFs* devfs = (DeviceFs*)s_devfs.get();
    devfs->registerDevice(name, ktl::shared_ptr<DeviceFile>(dev));
  }

}
