#include <loader.h>

Loader::Loader(const void* elfBinary)
{
  m_header = (Elf64Header*)elfBinary;
}

bool Loader::isValidBinary() const
{
  if (m_header == nullptr)
    return false;
  if (m_header->magic != 0x464c457f)
    return false;
  return true;
}

size_t Loader::getEntryPoint() const
{
  return (size_t)m_header->entry;
}
