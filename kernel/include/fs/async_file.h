#pragma once

#include <dev/devices.h>

namespace fs {

  class AsyncFile : public dev::Device
  {
    public:
      AsyncFile() = default;
      ~AsyncFile() = default;

    protected:
      ssize_t read(char* buf, size_t len) final {
        return -1;
      }

      void onRead(char* buf, size_t len) { }
  };

}
