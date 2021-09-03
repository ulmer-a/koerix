#pragma once

#include <fs/file.h>

namespace dev {

  class Device : public fs::File
  {
    public:
      Device();
      ~Device();

      virtual const char* getName() const = 0;
  };

  void setup();

  dev::Device* findDevice(const char* name);
  void registerDevice(dev::Device* chardev);
  void unregisterDevice(dev::Device* chardev);

}
