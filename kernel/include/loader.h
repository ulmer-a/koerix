#pragma once

#include <types.h>
#include <elf.h>

class Loader
{
  public:
    Loader(const void* elfBinary);

    bool isValidBinary() const;
    size_t getEntryPoint() const;

  private:
    Elf64Header* m_header;
};
