// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <module.h>
#include <string.h>
#include <x86/stivale.h>

extern stivale_struct* s_stivale;

void* find_module(const char* name)
{
  auto module = (stivale_module*)s_stivale->modules;
  for (size_t i = 0; i < s_stivale->module_count; i++)
  {
    if (strcmp(module->string, name) == 0)
      return (void*)module->begin;
    module = (stivale_module*)module->next;
  }
  return nullptr;
}
