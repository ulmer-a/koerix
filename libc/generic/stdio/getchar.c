// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include "getc.h"

int getchar(void)
{
  return do_getc(stdin);
}
