// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <fs/file.h>
#include <sync/mutex.h>
#include <shared_ptr.h>
#include <vector.h>

namespace fs {

  class Dir : public File
  {
    public:
      struct Direntry
      {
        char name[256];
        ktl::shared_ptr<File> file;
      };

      Dir() = default;
      ~Dir() = default;

      bool isMountPoint() const;
      bool mount(ktl::shared_ptr<Dir> fs, int& error);

      bool isDir() const final { return true; }
      ktl::shared_ptr<File> lookup(const char* path, int& error);

    protected:
      ssize_t read(char* buf, size_t len) override;
      ssize_t write(char* buf, size_t len) override;

      void addFile(const char* name, ktl::shared_ptr<File> file);

    private:
      bool matchesFileName(const char* path, const char* reference);
      ktl::shared_ptr<File> doLookup(const char* path, int& error);

      Mutex m_fileLock;
      ktl::vector<Direntry*> m_files;

      Mutex m_mountLock;
      ktl::shared_ptr<Dir> m_mountOverlay;
  };

}
