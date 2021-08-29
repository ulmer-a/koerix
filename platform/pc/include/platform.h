#pragma once

#include <asm.h>

// platform-native debug print function
// in this case: qemu and bochs 0xe9 port
extern void nprint(const char* str);