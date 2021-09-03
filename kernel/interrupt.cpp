#include <interrupt.h>

struct IrqEntry
{
    void(*func)(void*);
    void* this_ptr;
};

static IrqEntry irqs[16];

void handle_irq(size_t id)
{
  auto& entry = irqs[id];
  if (entry.func != nullptr)
  {
    entry.func(entry.this_ptr);
  }
}

void registerIrq(size_t id, void* func, void* this_ptr)
{
  irqs[id].func = (void(*)(void*))func;
  irqs[id].this_ptr = this_ptr;
}
