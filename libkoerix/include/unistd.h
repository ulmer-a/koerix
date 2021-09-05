#pragma once

#include <sys/types.h>

void __attribute((noreturn)) _exit(int status);

ssize_t read(int fd, char* buffer, size_t len);
ssize_t write(int fd, char* buffer, size_t len);
