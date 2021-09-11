#pragma once

extern "C" bool enable_nx();
extern "C" bool sse_enable();

extern bool s_nxEnabled;
extern bool s_sseEnabled;

/* on every context switch the fpu flag is cleared
 * instead of saving the whole fpu context. this allows
 * for better performance. */
extern "C" void resetFpuFlag();
