// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#include <math.h>

double sqrt(double x)
{
  __asm__ ("sqrtsd %1, %0" : "=x"(x) : "x"(x));
  return x;
}
