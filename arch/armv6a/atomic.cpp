// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <arch/asm.h>
#include <types.h>

size_t xchg(size_t val, size_t* mem)
{
#warning xchg() not implemented
  size_t temp = *mem;
  *mem = val;
  return temp;
}

size_t atomic_add(size_t* mem, ssize_t increment)
{
  size_t tmp, result;
  __asm__ volatile (
    "@ atomic_add\n"
    "1:     ldrex   %0, [%3]\n"
    "       add     %0, %0, %4\n"
    "       strex   %1, %0, [%3]\n"
    "       teq     %1, #0\n"
    "       bne     1b"
    : "=&r" (result), "=&r" (tmp), "+Qo" (mem)
    : "r" (&mem), "Ir" (increment)
    : "cc");
  return result;
}

uint8_t atomic_add8(uint8_t* mem, int8_t increment)
{
#warning atomic_add8() not implemented
  return *mem + increment;
}
