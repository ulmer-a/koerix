#include <syscalls.h>
#include <context.h>
#include <errno.h>
#include <debug.h>

static void* syscalls[] = {
  (void*)sys_exit,          // 0
  (void*)sys_thread_exit,   // 1
  (void*)sys_read,          // 2
  (void*)sys_write,         // 3
  nullptr,                  // 4
  nullptr,                  // 5
  (void*)set_thread_ptr,    // 6
  (void*)get_thread_ptr,    // 7
  nullptr,                  // 8
  (void*)fb_info,           // 9
  (void*)sys_thread_create, // 10
  (void*)sys_thread_count   // 11
};

void do_syscall(struct IrqContext* ctx)
{
  /* check for an invalid system call number */
  if (ctx->rax >= sizeof(syscalls) / sizeof(void*))
  {
    ctx->rax = -ENOSYS;
    return;
  }

  /* resolve the function address of the system call */
  void* syscall_addr = syscalls[ctx->rax];
  if (syscall_addr == nullptr)
  {
    ctx->rax = -ENOSYS;
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
