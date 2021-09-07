#include <debug.h>
#include <string.h>
#include <platform.h>
#include <term.h>
#include <spin.h>
#include <mm.h>
#include <pc/serial.h>

using namespace debugging;

static Spinlock s_debugLock;
static bool s_initialized = false;
static pc::SerialPort s_serial;

void panic(const char* message)
{
  cli();
  debug() << "**** kernel panic: " << message << "\n\n\n";
  for (;;) { hlt(); }
}

void debug_init()
{
  Terminal::init();
  new (&s_debugLock) Spinlock();
  new (&s_serial) pc::SerialPort(pc::SerialPort::COM2, true);
  s_initialized = true;
}

DebugStream::~DebugStream()
{
  if (!s_initialized)
    return;

  size_t len = strlen(m_buffer);
  s_debugLock.lock();
#ifdef DEBUG
  /* only print to QEMU debug IO port in debug mode */
  qemu_print(m_buffer);
  s_serial.write(m_buffer, len);
#endif

  /* kernel output will always go to the main terminal,
   * if there is one. */
  auto mainTerm = Terminal::getMainTerm();
  if (mainTerm != nullptr)
  {
    mainTerm->write(m_buffer, len);
  }
  s_debugLock.unlock();
}
