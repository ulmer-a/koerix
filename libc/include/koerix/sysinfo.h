// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#define __NEED_size_t
#include <bits/alltypes.h>
#include "../../../kernel/include/bits.h"

int sysinfo(int type, size_t* value);
