// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>
#include <elf.h>

class AddrSpace;

class Loader
{
  public:
    Loader(const void* elfBinary);

    void* entryAddr() const;
    bool isValid() const;

    bool load(size_t addr, AddrSpace& vspace) const;

  private:
    bool m_valid;
    bool isValidBinary() const;

    const void* m_elfBinary;
    Elf64Header* m_header;

    size_t m_phtEntryCount;
    Elf64ProgHeaderTableEntry* m_pht;
};
