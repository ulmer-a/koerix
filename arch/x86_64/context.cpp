// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <context.h>
#include <string.h>
#include <debug.h>


IrqContext::IrqContext()
{
  assert(sizeof(IrqContext) == 24*8);
  memset((void*)this, 0, sizeof(IrqContext));
  rflags = 0x0200;
}

IrqContext::IrqContext(const IrqContext& self)
{
  memcpy((void*)this, (void*)&self, sizeof(IrqContext));
}

void IrqContext::newUserCtx()
{
  cs = 0x1b;
  ss = 0x23;
  fs = 0x23;
  gs = 0x23;
}

void IrqContext::newCompatUserCtx()
{
  cs = 0x2b;
  ss = 0x33;
  fs = 0x33;
  gs = 0x33;
}

void IrqContext::newKernelCtx()
{
  cs = 0x08;
  ss = 0x10;
  fs = 0x10;
  gs = 0x10;
}

void IrqContext::print()
{
  debug(EXCEPT)
    << "--- REGISTER DUMP (x86_64) ---\n"
    << DEBUG_HEX << "  rip=" << rip
    << ", rsp=" << rsp
    << ", err=" << error_id << "\n"
    << DEBUG_HEX << "  rax=" << rax
    << ", rbx=" << rbx
    << ", rcx=" << rcx << "\n"
    << DEBUG_HEX << "  rdx=" << rdx
    << ", rdi=" << rdi
    << ", rsi=" << rsi << "\n";
}
