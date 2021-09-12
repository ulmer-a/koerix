// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <fs/file.h>

namespace dev {

  class DeviceFile;

  void setup();

  void registerDevice(const char* name, dev::DeviceFile* dev);

}
