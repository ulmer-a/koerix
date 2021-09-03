#pragma once

#include <types.h>

void registerIrq(size_t id, void* func, void* this_ptr);
