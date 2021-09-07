#pragma once

#include <types.h>
#include <shared_ptr.h>

namespace fs {

  class File;

  class FileDesc
  {
    public:
      FileDesc();
      FileDesc(File* file, bool write = false);
      ~FileDesc();

      inline bool valid() const { return m_valid; }

      ssize_t read(char* buf, size_t len);
      ssize_t write(char* buf, size_t len);

    private:
      bool m_valid;
      bool m_write;
      File* m_file;
  };

}
