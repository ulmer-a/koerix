// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

extern "C" bool enable_nx();
extern "C" bool sse_enable();

extern bool s_nxEnabled;
extern bool s_sseEnabled;

/* on every context switch the fpu flag is cleared
 * instead of saving the whole fpu context. this allows
 * for better performance. */
extern "C" void resetFpuFlag();
