// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>

class Task;
class UserTask;

namespace sched {
  void _INIT setup(Task* initialTask);

  void yield();

  bool isEnabled();
  Task* currentTask();
  size_t currentPid();
  UserTask* currentUserTask();

  size_t taskCount();
  size_t getIdleClks();

  void insertTask(Task* task);
  void removeTask(Task* task);
}
