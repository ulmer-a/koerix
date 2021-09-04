#include <loader.h>
#include <pagemap.h>
#include <string.h>

Loader::Loader(const void* elfBinary)
  : m_elfBinary(elfBinary)
{
  m_header = (Elf64Header*)elfBinary;
  m_pht = (Elf64ProgHeaderTableEntry*)
      ((char*)elfBinary + m_header->pht_pos);
  m_phtEntryCount = m_header->pht_entries;
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

bool Loader::load(size_t addr, AddrSpace& vspace) const
{
  for (size_t i = 0; i < m_phtEntryCount; i++)
  {
    auto& phtEntry = m_pht[i];
    if (addr < phtEntry.p_vaddr ||
        addr >= phtEntry.p_vaddr + phtEntry.p_memsz)
      continue;

    if (phtEntry.type != 1 /* load */)
      continue;

    /* allocate a page and clear it */
    size_t ppn = PageMap::get().alloc();
    char* data = (char*)PPN_TO_VIRT(ppn);
    memset((void*)data, 0, PAGE_SIZE);

    size_t length = PAGE_SIZE;
    size_t page_start_addr = addr & ~0xfff;
    void* destAddr = (void*)page_start_addr;
    if (phtEntry.p_vaddr > (size_t)destAddr)
    {
      destAddr = (void*)phtEntry.p_vaddr;
      length = PAGE_SIZE - (phtEntry.p_vaddr - page_start_addr);
    }

    size_t file_offset = phtEntry.p_offset
        + ((size_t)destAddr - phtEntry.p_vaddr);

    memcpy(destAddr, (void*)((char*)m_elfBinary + file_offset),
      PAGE_SIZE - length);

    int flags = AddrSpace::MAP_USER;
    if ((phtEntry.flags & BIT(0)) == 0) flags |= AddrSpace::MAP_NOEXEC;
    if ((phtEntry.flags & BIT(1)) != 0) flags |= AddrSpace::MAP_WRITE;
    vspace.map(addr >> PAGE_SHIFT, ppn, flags);
    return true;
  }

  return false;
}
