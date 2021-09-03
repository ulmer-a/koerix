#pragma once

#include <koerix/types.h>

#define STDIN_FD 0
#define STDOUT_FD 1
#define STDERR_FD 2

void __attribute__((noreturn)) exit(int status);

size_t getpid();

ssize_t read(int fd, char* buffer, size_t len);
ssize_t write(int fd, char* buffer, size_t len);
