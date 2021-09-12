// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <task.h>

class KernelTask : public Task
{
  public:
    KernelTask(void(*entry)(void*), void* arg = nullptr);
    ~KernelTask() = default;

    bool isUserTask() const final { return false; }
    void* arg() const { return m_argument; }

  private:
    void* m_argument;
};
