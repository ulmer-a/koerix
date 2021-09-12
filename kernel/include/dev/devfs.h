// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <fs/dir.h>

namespace dev {

  class DeviceFile : public fs::File
  {
    public:
      enum FileType
      {
        CHARDEV,
        BLOCKDEV
      };

      DeviceFile(size_t major, size_t minor);
      ~DeviceFile() = default;

      virtual int ioctrl(size_t cmd, size_t *arg);

      size_t major() const { return m_major; }
      size_t minor() const { return m_minor; }

      virtual FileType getType() const = 0;

    private:
      size_t m_major, m_minor;
  };

  class DeviceFs : public fs::Dir
  {
    public:
      DeviceFs() = default;
      ~DeviceFs() = default;

      static DeviceFs& get();

      void registerDevice(
          const char* name, ktl::shared_ptr<DeviceFile> device);
  };

}
