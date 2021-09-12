// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <fs/file.h>
#include <fs/fd.h>

using namespace fs;

FileDesc File::open(int& error)
{
  return FileDesc(this, true);
}
