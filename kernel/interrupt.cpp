// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

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
