#include <syscalls.h>
#include <context.h>
#include <errno.h>
#include <debug.h>

#include "../../kernel/syscall_list.h"

void do_syscall(IrqContext* ctx)
{
  /* check for an invalid system call number */
  if (ctx->eax >= sizeof(syscalls) / sizeof(void*))
  {
    ctx->eax = -ENOSYS;
    return;
  }

  /* resolve the function address of the system call */
  void* syscall_addr = syscalls[ctx->eax];
  if (syscall_addr == nullptr)
  {
    ctx->eax = -ENOSYS;
    return;
  }

  auto syscall = (size_t(*)(
        size_t arg1, size_t arg2, size_t arg3,
        size_t arg4, size_t arg5, size_t arg6
  ))syscall_addr;

  /* actually perform the call with the arguments from
   * the interrupt context structure */
  ctx->eax = syscall(
    ctx->ebx, ctx->ecx, ctx->rdx,
    ctx->edi, ctx->esi,  ctx->ebp
  );
}
