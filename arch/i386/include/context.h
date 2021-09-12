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
      return eax;
    }

    inline size_t& instructionPtr() {
      return eip;
    }

    inline size_t& stackPtr() {
      return esp;
    }

    inline size_t irq() {
      return irq_id;
    }

    inline size_t error() {
      return error_id;
    }

    inline size_t& arg1() {
      return ebx;
    }

    inline size_t& arg2() {
      return ecx;
    }

    inline size_t& arg3() {
      return edx;
    }

  private:
    size_t edi;
    size_t esi;
    size_t edx;
    size_t ecx;
    size_t ebx;
    size_t eax;
    size_t ebp;
    size_t gs;
    size_t fs;
    size_t irq_id;
    size_t error_id;
    size_t eip;
    size_t cs;
    size_t eflags;
    size_t esp;
    size_t ss;
};
