// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>

class _PACKED IrqContext
{
    friend void do_syscall(IrqContext* ctx);

  public:
    explicit IrqContext();
    explicit IrqContext(const IrqContext& self);

    void newUserCtx();
    void newCompatUserCtx();
    void newKernelCtx();

    void print();

    inline size_t& returnValue() {
      return rax;
    }

    inline size_t& instructionPtr() {
      return rip;
    }

    inline size_t& stackPtr() {
      return rsp;
    }

    inline size_t irq() {
      return irq_id;
    }

    inline size_t error() {
      return error_id;
    }

    inline size_t& arg1() {
      return rdi;
    }

    inline size_t& arg2() {
      return rsi;
    }

    inline size_t& arg3() {
      return rdx;
    }

  private:
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
    size_t irq_id;
    size_t error_id;
    size_t rip;
    size_t cs;
    size_t rflags;
    size_t rsp;
    size_t ss;
};
