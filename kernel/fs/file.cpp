// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <fs/file.h>
#include <fs/fd.h>
#include <arch/asm.h>

using namespace fs;

File::File()
  : m_refs(0)
{

}

FileDesc File::open(int& error)
{
  return FileDesc(this, true);
}

void File::addRef()
{
  atomic_add(&m_refs, 1);
}

void File::release()
{
  assert(m_refs > 0);
  size_t refs_before = atomic_add(&m_refs, -1);
  if (refs_before == 1)
    closed();
}
