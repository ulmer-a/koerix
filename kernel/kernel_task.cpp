// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <kernel_task.h>
#include <addr_space.h>
#include <context.h>
#include <debug.h>

static void ktask_runtime(KernelTask* task, void(*func)(void*))
{
  debug() << "kernel task #" << task->tid() << " started executing\n";
  func(task->arg());
  task->exit();
}

KernelTask::KernelTask(void(*entry)(void* arg), void* arg)
  : Task(AddrSpace::kernel())
  , m_argument(arg)
{
  context()->newKernelCtx();
  context()->instructionPtr() = (size_t)ktask_runtime;
  context()->stackPtr() = kernelStackPtr();
  context()->arg1() = (size_t)this;
  context()->arg2() = (size_t)entry;

  debug() << "created new kernel task with TID " << tid() << "\n";
}
