// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <syscalls.h>
#include <context.h>
#include <errno.h>
#include <debug.h>
#include <scheduler.h>

#include "../../kernel/syscall_list.h"

static void invalidSyscall(IrqContext* ctx)
{
  debug(SYSCALL) << "PID " << sched::currentPid()
                 << ": invalid system call: #"
                 << ctx->returnValue() << "\n";
}

void do_syscall(IrqContext* ctx)
{
  /* check for an invalid system call number */
  if (ctx->rax >= sizeof(syscalls) / sizeof(void*))
  {
    ctx->rax = -ENOSYS;
    invalidSyscall(ctx);
    return;
  }

  /* resolve the function address of the system call */
  void* syscall_addr = syscalls[ctx->rax];
  if (syscall_addr == nullptr)
  {
    ctx->rax = -ENOSYS;
    invalidSyscall(ctx);
    return;
  }

  auto syscall = (size_t(*)(
        size_t arg1, size_t arg2, size_t arg3,
        size_t arg4, size_t arg5, size_t arg6
  ))syscall_addr;

  /* actually perform the call with the arguments from
   * the interrupt context structure */
  ctx->rax = syscall(
    ctx->rdi, ctx->rsi, ctx->rdx,
    ctx->rcx, ctx->r8,  ctx->r9
  );
}
