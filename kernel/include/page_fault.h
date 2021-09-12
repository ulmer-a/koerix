// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>

enum FaultFlags
{
  /* only add more flags at the bottom of the structure! */
  PF_PRESENT      = BIT(0), // page was present, so the fault
                            // was caused by protection violation
  PF_WRITE        = BIT(1), // caused by a write access
  PF_USER         = BIT(2), // fault caused by user mode code
  PF_RESERVED     = BIT(3), // fault was caused by a reserved bit set
  PF_CODE         = BIT(4), // fault caused by fetching code (not data)
};

bool handlePageFault(size_t addr, FaultFlags flags);
