// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>

extern "C" {
  size_t xchg(size_t val, size_t* mem);
  uint8_t atomic_add8(uint8_t* mem, int8_t increment);
  size_t atomic_add(size_t* mem, ssize_t increment);
}
