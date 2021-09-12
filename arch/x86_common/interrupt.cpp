#include <types.h>
#include <debug.h>
#include <context.h>
#include <platform.h>
#include <arch/asm.h>
#include <page_fault.h>
#include <string.h>
#include <arch/asm.h>
#include <scheduler.h>
#include <user_task.h>

#define EXC_FPU_SSE     7
#define EXC_PAGEFAULT   14

extern void fpuFault();
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
        fpuFault();
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

        debug() << "fault: " << addr
                << ", present=" << (ctx->error() & PF_PRESENT ? "y" : "n")
                << ", user=" << (ctx->error() & PF_USER ? "y" : "n")
                << ", write=" << (ctx->error() & PF_WRITE ? "y" : "n")
                << ", reserved=" << (ctx->error() & PF_RESERVED ? "y" : "n")
                << ", data=" << (ctx->error() & PF_CODE ? "n" : "y") << "\n";

        if (handlePageFault((size_t)addr, (FaultFlags)ctx->error())) {
          return ctx;
        }
      }

      debug() << "(!!) exception #" << ctx->irq()
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
