#pragma once

#include <types.h>

class Task;
class UserTask;

namespace sched {
  void _INIT setup(Task* initialTask);

  void yield();

  bool isEnabled();
  Task* currentTask();
  UserTask* currentUserTask();

  size_t taskCount();
  size_t getIdleClks();

  void insertTask(Task* task);
  void removeTask(Task* task);
}
