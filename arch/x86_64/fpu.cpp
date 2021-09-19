// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <fpu.h>
#include <mm.h>
#include <user_task.h>
#include <scheduler.h>
#include <arch/asm.h>
#include <lib/string.h>

namespace fpu {

  struct ContextStruct
  {
    // TODO: fill in the actual layout
    unsigned char data[512];
  } _PACKED;

  Context::Context()
  {
    /* allocate 16 extra bytes to be able to find
     * a 16byte-aligned location within m_data. */
    assert(sizeof(ContextStruct) == 512);
    m_impl.m_context = (ContextStruct*)(((size_t)m_impl.m_data & ~0xf) + 16);
  }

  Context* Context::clone()
  {
    auto ctx = new Context();
    memcpy((void*)ctx->m_impl.m_context, (void*)m_impl.m_context,
      sizeof(ContextStruct));
    return ctx;
  }

  void Context::save()
  {
    __asm__ volatile ("fxsave %0;" : : "m"(*m_impl.m_context));
  }

  void Context::restore() const
  {
    __asm__ volatile ("fxrstor %0;" : : "m"(*m_impl.m_context));
  }

}
