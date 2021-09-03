#pragma once

#include <types.h>

namespace fs {

  class FileDesc;

  class File
  {
      friend class FileDesc;

    public:
      File() = default;
      virtual ~File() = default;

    protected:
      virtual ssize_t read(char* buf, size_t len) = 0;
      virtual ssize_t write(char* buf, size_t len) = 0;
  };

}
