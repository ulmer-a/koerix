#pragma once

class IrqContext
{
  public:
    static void newKernelCtx();
    static void newUserCtx();

    size_t& stackPtr();
    size_t& instructionPtr();
    size_t& arg1();
    size_t& arg2();
};