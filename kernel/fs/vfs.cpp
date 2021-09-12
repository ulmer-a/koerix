// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <fs/vfs.h>
#include <fs/dir.h>
#include <fs/file.h>
#include <fs/fd.h>
#include <errno.h>
#include <string.h>

namespace fs {

  static ktl::shared_ptr<Dir> s_root;

  ktl::shared_ptr<File> lookup(const char* path, int& error)
  {
    if (s_root == nullptr)
    {
      error = ENOENT;
      return ktl::shared_ptr<fs::File>();
    }

    return s_root->lookup(path, error);
  }

  FileDesc open(const char* filename, int& error)
  {
    auto file = lookup(filename, error);
    if (file == nullptr)
      return FileDesc();

    return file->open(error);
  }

  bool mount(const char* filename, ktl::shared_ptr<Dir> fs, int& error)
  {
    if (strcmp(filename, "/") == 0)
    {
      s_root = fs;
      return true;
    }

    auto file = lookup(filename, error);
    if (file == nullptr)
      return false;

    if (!file->isDir())
    {
      error = ENOTDIR;
      return false;
    }

    auto dir = (Dir*)file.get();
    return dir->mount(fs, error);
  }

}
