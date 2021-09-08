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
