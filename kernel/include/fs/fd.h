// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>
#include <lib/shared_ptr.h>

namespace fs {

  class File;

  class FileDesc
  {
    public:
      FileDesc();
      FileDesc(File* file, bool write = false);
      FileDesc(const FileDesc& fd);
      FileDesc& operator=(const FileDesc& fd);
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
