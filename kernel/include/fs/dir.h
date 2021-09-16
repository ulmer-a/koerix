// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <fs/file.h>
#include <sync/mutex.h>
#include <lib/shared_ptr.h>
#include <lib/vector.h>

namespace fs {

  class Dir : public File
  {
    public:
      struct Direntry
      {
        char name[256];
        lib::shared_ptr<File> file;
      };

      Dir() = default;
      ~Dir() = default;

      bool isMountPoint() const;
      bool mount(lib::shared_ptr<Dir> fs, int& error);

      bool isDir() const final { return true; }
      lib::shared_ptr<File> lookup(const char* path, int& error);

    protected:
      ssize_t read(char* buf, size_t len) override;
      ssize_t write(char* buf, size_t len) override;

      void addFile(const char* name, lib::shared_ptr<File> file);

    private:
      bool matchesFileName(const char* path, const char* reference);
      lib::shared_ptr<File> doLookup(const char* path, int& error);

      Mutex m_fileLock;
      lib::vector<Direntry*> m_files;

      Mutex m_mountLock;
      lib::shared_ptr<Dir> m_mountOverlay;
  };

}
