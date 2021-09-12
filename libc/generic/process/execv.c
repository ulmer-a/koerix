// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#include <unistd.h>

extern char **__environ;

int execv(const char *path, char *const argv[])
{
  return execve(path, argv, __environ);
}
