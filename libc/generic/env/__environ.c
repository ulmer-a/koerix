// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#include <unistd.h>

char **__environ = 0;
weak_alias(__environ, ___environ);
weak_alias(__environ, _environ);
weak_alias(__environ, environ);
