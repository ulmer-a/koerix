#include <types.h>
#include <debug.h>
#include <context.h>
#include <platform.h>
#include <arch/asm.h>
#include <page_fault.h>
#include <string.h>
#include <arch/asm.h>

static void* getPageFaultAddr()
{
    void* addr;
    __asm__ volatile ("mov %%cr2, %0;" : "=r"(addr));
    return addr;
}

namespace sched {
  IrqContext* schedule(IrqContext* ctx);
}

extern void handle_irq(size_t id);

extern "C" IrqContext* x86_irq_handler(IrqContext* ctx)
{
  if (ctx->irq < 32)
  {
    if (ctx->irq == 0x1f)
    {
      ctx = sched::schedule(ctx);
    }
    else
    {
      if (ctx->irq == 14)
      {
        sti();
        void* addr = getPageFaultAddr();
        if (handlePageFault((size_t)getPageFaultAddr(), (FaultFlags)ctx->error)) {
          cli();
          return ctx;
        }

        debug() << "unhandled page fault @ " << addr << "\n";
        debug() << "present=" << (ctx->error & PF_PRESENT ? "y" : "n")
                << ", user=" << (ctx->error & PF_USER ? "y" : "n")
                << ", write=" << (ctx->error & PF_WRITE ? "y" : "n")
                << ", reserved=" << (ctx->error & PF_RESERVED ? "y" : "n")
                << ", data=" << (ctx->error & PF_CODE ? "n" : "y") << "\n";
      }

      debug() << "(!!) exception #" << ctx->irq << " (" << strexcept(ctx->irq) << ")\n";
      debug() << DEBUG_HEX << "rip=" << ctx->rip << ", rsp=" << ctx->rsp << ", err=" << ctx->error << "\n";
      panic("unhandled exception");
    }
  }
  else if (ctx->irq < 48)
  {
    size_t irq_id = ctx->irq - 32;
    if (irq_id == 0)
    {
      ctx = sched::schedule(ctx);
    }

    handle_irq(irq_id);

    if (irq_id >= 8)
      outb(0xa0, 0x20);
    outb(0x20, 0x20);
  }
  else
  {
    assert(false);
  }

  return ctx;
}
