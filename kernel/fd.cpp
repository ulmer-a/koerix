// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <fs/fd.h>
#include <fs/file.h>
#include <errno.h>

using namespace fs;


FileDesc::FileDesc()
  : m_valid(false)
  , m_write(false)
{

}

FileDesc::FileDesc(File* file, bool write)
  : m_valid(true)
  , m_file(file)
  , m_write(write)
{
  assert(m_file != nullptr);
  m_file->addRef();
}

FileDesc::FileDesc(const FileDesc& fd)
  : m_valid(fd.m_valid)
  , m_file(fd.m_file)
  , m_write(fd.m_write)
{
  if (m_file)
    m_file->addRef();
}

FileDesc& FileDesc::operator=(const FileDesc& fd)
{
  if (m_file)
    m_file->release();

  m_file = fd.m_file;
  m_valid = fd.m_valid;
  m_write = fd.m_write;

  if (m_file)
    m_file->addRef();

  return *this;
}

FileDesc::~FileDesc()
{
  if (m_file)
    m_file->release();
}

ssize_t FileDesc::read(char* buf, size_t len)
{
  assert(m_file != nullptr);
  return m_file->read(buf, len);
}

ssize_t FileDesc::write(char* buf, size_t len)
{
  if (!m_write) {
    return -EPERM;
  }

  assert(m_file != nullptr);
  return m_file->write(buf, len);
}
