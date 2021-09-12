// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

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
