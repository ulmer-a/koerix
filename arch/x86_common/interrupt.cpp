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

extern "C" IrqContext* x86_irq_handler(IrqContext* ctx)
{
    debug() << "IRQ (#" << ctx->irq << ")!\n";
    debug() << "code location: " << (void*)ctx->rip << ", error=" << ctx->error << "\n";
    debug() << "fault addr: " << getPageFaultAddr() << "\n";
    for (;;) { hlt(); }
    return ctx;
}
