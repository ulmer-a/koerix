// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include "putc.h"

int putchar(int c)
{
  return do_putc(c, stdout);
}
