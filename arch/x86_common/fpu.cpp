// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <x86/fpu.h>
#include <mm.h>
#include <user_task.h>
#include <scheduler.h>
#include <x86/asm.h>
#include <string.h>

static UserTask* s_fpuTask = nullptr;

extern "C" void enableFpuCalls();

struct FpuContextStruct
{
  // TODO: fill in the actual layout
  unsigned char data[512];
} _PACKED;

FpuContext::FpuContext()
{
  /* allocate 16 extra bytes to be able to find
   * a 16byte-aligned location within m_data. */
  assert(sizeof(FpuContextStruct) == 512);
  m_context = (FpuContextStruct*)(((size_t)m_data & ~0xf) + 16);
}

FpuContext* FpuContext::clone()
{
  auto ctx = new FpuContext();
  memcpy((void*)ctx->m_context, (void*)m_context, sizeof(FpuContextStruct));
  return ctx;
}

void fpuFault()
{
  /* the current user task has accessed FPU/SSE registers or
   * instructions. for performance reasons we only save and
   * restore the FPU/SSE context on demand, which means that
   * the FPU/SSE context is not saved/restored on each individual
   * context switch. */

  enableFpuCalls();

  auto currentTask = sched::currentUserTask();
  if (s_fpuTask != currentTask)
  {
    if (s_fpuTask != nullptr)
      s_fpuTask->getFpuContext().save();
    currentTask->getFpuContext().restore();
    s_fpuTask = currentTask;
  }
}

void FpuContext::save()
{
  __asm__ volatile ("fxsave %0;" : : "m"(*m_context));
}

void FpuContext::restore() const
{
  __asm__ volatile ("fxrstor %0;" : : "m"(*m_context));
}

void fpuClear()
{
  cli();
  if (s_fpuTask == sched::currentTask())
  {
    s_fpuTask = nullptr;
    resetFpuFlag();
  }
  sti();
}
