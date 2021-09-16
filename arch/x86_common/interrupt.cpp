// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <types.h>
#include <debug.h>
#include <context.h>
#include <platform.h>
#include <arch/asm.h>
#include <page_fault.h>
#include <lib/string.h>
#include <arch/asm.h>
#include <scheduler.h>
#include <user_task.h>
#include <fpu.h>

#define EXC_FPU_SSE     7
#define EXC_PAGEFAULT   14

extern void do_syscall(IrqContext* ctx);

uint64_t s_timerTicks = 0;

static void* getPageFaultAddr()
{
    void* addr;
    __asm__ volatile ("mov %%cr2, %0;" : "=r"(addr));
    return addr;
}

namespace sched {
namespace core {
  IrqContext* schedule(IrqContext* ctx);
}
}

extern void handle_irq(size_t id);

extern "C" IrqContext* x86_irq_handler(IrqContext* ctx)
{
  if (ctx->irq() < 32)
  {
    if (ctx->irq() == 0x1f)
    {
      ctx = sched::core::schedule(ctx);
    }
    else
    {
      if (ctx->irq() == EXC_FPU_SSE)
      {
        fpu::onFault();
        return ctx;
      }
      else if (ctx->irq() == EXC_PAGEFAULT)
      {
        /* make sure to grab the page fault address while we're
         * still executing in an interrupt context, so we don't
         * take the address from some other page fault that might
         * have occurred in the meantime.  */
        void* addr = getPageFaultAddr();
        sti();

        if (handlePageFault((size_t)addr, (FaultFlags)ctx->error())) {
          return ctx;
        }

        debug(PAGEFAULT)
          << "fault @ " << addr << ": "
          << (ctx->error() & PF_PRESENT ? "protection, " : "non-present, ")
          << (ctx->error() & PF_USER ? "user, " : "kernel,")
          << (ctx->error() & PF_WRITE ? "write, " : "read, ")
          << (ctx->error() & PF_RESERVED ? "reserved, " : "")
          << (ctx->error() & PF_CODE ? "code" : "data") << "\n";
      }

      debug(EXCEPT) << "(!!) exception #" << ctx->irq()
              << " (" << strexcept(ctx->irq()) << ")\n";
      ctx->print();

      panic("unhandled exception");
    }
  }
  else if (ctx->irq() < 48)
  {
    size_t irq_id = ctx->irq() - 32;
    if (irq_id == 0)
    {
      s_timerTicks += 1;
      ctx = sched::core::schedule(ctx);
    }

    handle_irq(irq_id);

    if (irq_id >= 8)
      outb(0xa0, 0x20);
    outb(0x20, 0x20);
  }
  else if (ctx->irq() == 0x80)
  {
    sti();
    sched::currentTask()->setContext(ctx);
    do_syscall(ctx);
  }
  else
  {
    assert(false);
  }

  return ctx;
}
