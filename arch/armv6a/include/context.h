// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>

class _PACKED IrqContext
{
    friend void do_syscall(IrqContext* ctx);

  public:
    explicit IrqContext() { }
    explicit IrqContext(const IrqContext& self);

    void newUserCtx() { }
    void newCompatUserCtx() { }
    void newKernelCtx() { }

    void print();

    inline size_t& returnValue() {
      return r0;
    }

    inline size_t& instructionPtr() {
      return pc;
    }

    inline size_t& stackPtr() {
      return r14;
    }

    inline size_t irq() {
      return irq_id;
    }

    inline size_t error() {
      return error_id;
    }

    inline size_t& arg1() {
      return r0;
    }

    inline size_t& arg2() {
      return r1;
    }

    inline size_t& arg3() {
      return r2;
    }

  private:
    size_t pc;
    size_t r14;
    size_t r13;
    size_t r12;
    size_t r11;
    size_t r10;
    size_t r9;
    size_t r8;
    size_t r7;
    size_t r6;
    size_t r5;
    size_t r4;
    size_t r3;
    size_t r2;
    size_t r1;
    size_t r0;
    size_t cpsr;
    size_t irq_id;
    size_t error_id;
};
