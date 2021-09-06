#include <term.h>
#include <mm.h>

ktl::shared_ptr<Terminal> Terminal::s_mainTerm;

Terminal::Terminal(fs::FileDesc& fd)
  : m_fd(fd)
{

}

void Terminal::init()
{
  new (&s_mainTerm) ktl::shared_ptr<Terminal>();
}

ssize_t Terminal::read(char* buffer, size_t len)
{
  return m_fd.read(buffer, len);
}

ssize_t Terminal::write(char* buffer, size_t len)
{
  return m_fd.write(buffer, len);
}
