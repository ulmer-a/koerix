#include <term.h>

Terminal* Terminal::s_mainTerm = nullptr;

Terminal::Terminal(fs::FileDesc& fd)
  : m_fd(fd)
{

}

ssize_t Terminal::read(char* buffer, size_t len)
{
  return m_fd.read(buffer, len);
}

ssize_t Terminal::write(char* buffer, size_t len)
{
  return m_fd.write(buffer, len);
}
