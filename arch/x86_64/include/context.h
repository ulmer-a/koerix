#pragma once

#include <types.h>

struct IrqContext
{
  size_t r15;
  size_t r14;
  size_t r13;
  size_t r12;
  size_t r11;
  size_t r10;
  size_t r9;
  size_t r8;
  size_t rdi;
  size_t rsi;
  size_t rdx;
  size_t rcx;
  size_t rbx;
  size_t rax;
  size_t rbp;
  size_t gs;
  size_t fs;
  size_t irq;
  size_t error;
  size_t rip;
  size_t cs;
  size_t rflags;
  size_t rsp;
  size_t ss;
} _PACKED;

static inline void setInstructionPointer(IrqContext* ctx, size_t ip)
{
  ctx->rip = ip;
}

static inline void initArchContext(IrqContext* ctx, size_t ip,
                size_t sp, size_t arg1, size_t arg2)
{
  /* The instruction pointer is initialized with the entry
   * address, the stack pointer with the top of the kernel
   * stack. */
  ctx->rip = ip;
  ctx->rsp = sp;

  /* Code and data segment selectors. The GDT is setup
   * in a way so that 0x08 will always point to a descriptor
   * for kernel code and 0x10 will always point to one
   * for kernel data. */
  ctx->cs = 0x08;
  ctx->ss = 0x10;
  ctx->fs = 0x10;
  ctx->gs = 0x10;

  /* Arguments: Calling convention implies that argument 1 and
   * two must be located in %rdi, %rsi registers. */
  ctx->rdi = arg1;
  ctx->rsi = arg2;

  /* enable interrupts */
  ctx->rflags = 0x0200;

  /* All other registers have been initialized with zeroes
   * before by the Task class constructor */
}
