// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include "getc.h"

int getc(FILE *f)
{
  return do_getc(f);
}

weak_alias(getc, _IO_getc);
