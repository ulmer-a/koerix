// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <dev/devfs.h>
#include <errno.h>

using namespace dev;

DeviceFile::DeviceFile(size_t major, size_t minor)
  : m_major(major)
  , m_minor(minor)
{

}

int DeviceFile::ioctrl(size_t cmd, size_t *arg)
{
  return -ENOTTY;
}

void DeviceFs::registerDevice(
    const char* name, ktl::shared_ptr<DeviceFile> device)
{
  addFile(name, *(ktl::shared_ptr<File>*)&device);
}
