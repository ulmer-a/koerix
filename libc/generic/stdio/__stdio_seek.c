// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#include "stdio_impl.h"
#include <unistd.h>

off_t __stdio_seek(FILE *f, off_t off, int whence)
{
	return __lseek(f->fd, off, whence);
}
