// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#include <ctype.h>
#undef isdigit

int isdigit(int c)
{
  return (unsigned)c-'0' < 10;
}

int __isdigit_l(int c, locale_t l)
{
  return isdigit(c);
}

weak_alias(__isdigit_l, isdigit_l);
