#include <types.h>
#include <debug.h>
#include <context.h>
#include <platform.h>

extern "C" IrqContext* x86_irq_handler(IrqContext* ctx)
{
    debug() << "IRQ (#" << ctx->irq << ")!\n";
    for (;;) { hlt(); }
    return ctx;
}
