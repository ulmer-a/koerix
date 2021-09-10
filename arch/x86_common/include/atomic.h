#pragma once

#include <types.h>

size_t xchg(size_t val, size_t* mem);
uint8_t atomic_add8(uint8_t* mem, int8_t increment);
size_t atomic_add(size_t* mem, ssize_t increment);
