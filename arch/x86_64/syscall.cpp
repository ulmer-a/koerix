#include <syscalls.h>
#include <context.h>
#include <errno.h>
#include <debug.h>

static void* syscalls[] = {
  (void*)sys_exit,          // 0
  (void*)sys_thread_exit,   // 1
  (void*)sys_read,          // 2
  (void*)sys_write          // 3
};

void do_syscall(struct IrqContext* ctx)
{
  /* check for an invalid system call number */
  if (ctx->rax >= sizeof(syscalls))
  {
    ctx->rax = -ENOSYS;
    return;
  }

  /* resolve the function address of the system call */
  void* syscall_addr = syscalls[ctx->rax];
  assert(syscall_addr != nullptr);
  auto syscall = (size_t(*)(
        size_t arg1, size_t arg2, size_t arg3,
        size_t arg4, size_t arg5, size_t arg6
  ))syscall_addr;

  /* actually perform the call with the arguments from
   * the interrupt context structure */
  ctx->rax = syscall(
    ctx->r8,  ctx->r9,  ctx->r10,
    ctx->r11, ctx->r12, ctx->r13
  );
}
