#include <debug.h>
#include <string.h>
#include <platform.h>
#include <term.h>
#include <spin.h>
#include <mm.h>

using namespace debugging;

static Spinlock s_debugLock;
static bool s_debugInitialized = false;

void panic(const char* message)
{
  cli();
  debug() << "**** kernel panic: " << message << "\n\n\n";
  for (;;) { hlt(); }
}

DebugStream::~DebugStream()
{
  if (!s_debugInitialized) {
    new (&s_debugLock) Spinlock();
  }

  s_debugLock.lock();
#ifdef DEBUG
  /* only print to QEMU debug IO port in debug mode */
  qemu_print(m_buffer);
#endif

  /* kernel output will always go to the main terminal,
   * if there is one. */
  auto mainTerm = Terminal::getMainTerm();
  if (mainTerm != nullptr)
  {
    size_t len = strlen(m_buffer);
    mainTerm->write(m_buffer, len);
  }
  s_debugLock.unlock();
}
