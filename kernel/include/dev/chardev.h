#pragma once

#include <types.h>

namespace dev {

  class CharDev
  {
    public:
      CharDev() = default;
      virtual ~CharDev() = default;

    protected:
      virtual int ioctrl(size_t cmd, size_t* arg) = 0;
      virtual ssize_t write(const char* buffer, size_t len) = 0;
  };

}
