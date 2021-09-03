#pragma once

#include <types.h>

namespace fs {

  class File;

  class FileDesc
  {
    public:
      FileDesc(File& file);
      ~FileDesc() = default;

      ssize_t read(char* buf, size_t len);
      ssize_t write(char* buf, size_t len);

    private:
      File& m_file;
  };

}
