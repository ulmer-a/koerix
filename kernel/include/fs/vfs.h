// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <lib/shared_ptr.h>

namespace fs {

  class FileDesc;
  class Dir;

  FileDesc open(const char* filename, int& error);

  bool mount(const char* filename, lib::shared_ptr<Dir> fs, int& error);

}
