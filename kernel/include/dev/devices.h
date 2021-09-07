#pragma once

#include <fs/file.h>

namespace dev {

  class DeviceFile;

  void setup();

  void registerDevice(const char* name, dev::DeviceFile* dev);

}
