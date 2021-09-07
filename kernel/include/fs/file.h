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

      virtual bool isDir() const { return false; }

      FileDesc open(int& error);

    protected:
      virtual ssize_t read(char* buf, size_t len) = 0;
      virtual ssize_t write(char* buf, size_t len) = 0;
  };

}
