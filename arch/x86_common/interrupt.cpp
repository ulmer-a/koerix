#include <types.h>
#include <debug.h>
#include <context.h>
#include <platform.h>
#include <asm.h>

static void* getPageFaultAddr()
{
    void* addr;
    __asm__ volatile ("mov %%cr2, %0;" : "=r"(addr));
    return addr;
}

namespace sched {
  IrqContext* schedule(IrqContext* ctx);
}

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
      debug() << "exception! (#" << ctx->irq << ")!\n";
      debug() << "code location: " << (void*)ctx->rip << ", error=" << ctx->error << "\n";
      debug() << "fault addr: " << getPageFaultAddr() << "\n";
      for (;;) { hlt(); }
    }
  }
  else if (ctx->irq < 48)
  {
    size_t irq_id = ctx->irq - 48;
    if (irq_id == 0)
    {
      ctx = sched::schedule(ctx);
    }

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
