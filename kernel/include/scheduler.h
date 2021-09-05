#pragma once

class Task;
class UserTask;

namespace sched {

    namespace init {
        void setup();
        void insertTask(Task* task);
    }

    bool isEnabled();
    void enable();
    void disable();

    void yield();

    Task* currentTask();
    UserTask* currentUserTask();
    void insertTask(Task* task);

}
