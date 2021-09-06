#pragma once

#define __NEED_off_t

#include <sys/types.h>
#include <features.h>

void _Noreturn _exit(int status);

ssize_t read(int fd, char* buffer, size_t len);
ssize_t write(int fd, char* buffer, size_t len);

int close(int fd);

#define lseek __lseek
off_t __lseek(int fd, off_t offset, int whence);
