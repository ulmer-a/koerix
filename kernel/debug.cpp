// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <debug.h>
#include <lib/string.h>
#include <platform.h>
#include <term.h>
#include <sync/spin.h>
#include <mm.h>

using namespace dbg;

static Spinlock s_debugLock;
static bool s_initialized = false;

void panic(const char* message)
{
  cli();
  debug(KERNEL) << "**** kernel panic: " << message << "\n\n\n";
  printStackTrace();
  for (;;) { hlt(); }
}

void debug_init()
{
  Terminal::init();
  new (&s_debugLock) Spinlock();
  debug_print_init();
  s_initialized = true;
}

DebugStream::~DebugStream()
{
  assert((size_t)m_destPtr - (size_t)m_buffer < BUFFER_SIZE);

  if (!s_initialized)
    return;

  if ((m_loglevel & OUTPUT_ENABLE) == 0)
    return;

  s_debugLock.lock();
#if defined(DEBUG)
  /* only print to QEMU debug IO port in debug mode */
  debug_print(m_buffer);
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
