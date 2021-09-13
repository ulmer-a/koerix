// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#include <string.h>

int strcmp(const char *l, const char *r)
{
  for (; *l==*r && *l; l++, r++);
  return *(unsigned char *)l - *(unsigned char *)r;
}
