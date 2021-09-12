#pragma once

#define __NEED_size_t
#include <bits/alltypes.h>
#include "../../../kernel/include/bits.h"

int sysinfo(int type, size_t* value);
