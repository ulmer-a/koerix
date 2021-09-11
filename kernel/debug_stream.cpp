#include <debug.h>
#include <string.h>

using namespace debugging;

DebugStream::DebugStream()
    : m_currentMode(DEC)
{
    m_buffer[0] = 0;
    m_destPtr = m_buffer;
}

DebugStream& DebugStream::operator<<(Modifier modif)
{
    m_currentMode = modif;
    return *this;
}

int DebugStream::getBase()
{
    switch (m_currentMode)
    {
    case HEX:
        return 16;
    case DEC:
        return 10;
    }
}

static char hex_char(size_t nibble)
{
  if (nibble > 0xF)
    return '*';
  if (nibble >= 0xA)
    return 'a' + nibble - 0xA;
  return '0' + nibble;
}

void DebugStream::basePrefix()
{
  if (getBase() == 16)
  {
    *m_destPtr++ = '0';
    *m_destPtr++ = 'x';
  }
}

DebugStream& DebugStream::operator<<(const char* str)
{
    strcat(m_destPtr, str);
    m_destPtr += strlen(str);
    return *this;
}

DebugStream& DebugStream::operator<<(char c)
{
  *m_destPtr++ = c;
  *m_destPtr = 0;
  return *this;
}

#ifndef i386
DebugStream& DebugStream::operator<<(size_t i)
{
  basePrefix();
  ultoa(i, m_destPtr, getBase());
  m_destPtr = m_buffer + strlen(m_buffer);
  return *this;
}
#endif

DebugStream& DebugStream::operator<<(void* p)
{
    *m_destPtr++ = '0';
    *m_destPtr++ = 'x';

    ssize_t ptr_nibbles = sizeof(void*) * 2;
    const size_t ptr = (size_t)p;
    while (--ptr_nibbles >= 0)
    {
        size_t nibble = (ptr >> (ptr_nibbles * 4)) & 0xf;
        *m_destPtr++ = hex_char(nibble);
    }
    *m_destPtr = 0;
    return *this;
}

DebugStream& DebugStream::operator<<(int32_t i)
{
  basePrefix();
  m_destPtr = itoa(i, m_destPtr, getBase());
  m_destPtr = m_buffer + strlen(m_buffer);
  return *this;
}

DebugStream &DebugStream::operator<<(uint32_t i)
{
  basePrefix();
  m_destPtr = utoa(i, m_destPtr, getBase());
  m_destPtr = m_buffer + strlen(m_buffer);
  return *this;
}

DebugStream& DebugStream::operator<<(uint64_t i)
{
  /* TODO: will probably cause problems on 32bit archs */
  return operator<<((size_t)i);
}
