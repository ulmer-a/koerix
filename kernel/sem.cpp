// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <sem.h>
#include <arch/asm.h>

Semaphore::Semaphore(size_t initialVal)
  : m_value(initialVal)
{

}

Semaphore::~Semaphore()
{

}

void Semaphore::post()
{
  ssize_t before = atomic_add(&m_value, 1);
  if (before == 0)
  {
    // wake up another thread
  }
}
