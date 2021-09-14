// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include "putc.h"

int putc(int c, FILE *f)
{
  return do_putc(c, f);
}

weak_alias(putc, _IO_putc);
