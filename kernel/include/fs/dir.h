#pragma once

#include <fs/file.h>
#include <mutex.h>
#include <shared_ptr.h>

namespace fs {

  class Dir : public File
  {
    public:
      Dir() = default;
      ~Dir() = default;

      bool isMountPoint() const;
      bool mount(ktl::shared_ptr<Dir> fs, int& error);

      bool isDir() const final { return true; }
      File* lookup(const char* path, int& error);

    protected:
      ssize_t read(char* buf, size_t len) override;
      ssize_t write(char* buf, size_t len) override;

    private:
      Mutex m_mountLock;
      ktl::shared_ptr<Dir> m_mountOverlay;
  };

}
