#pragma once

class Task;
class UserTask;

namespace sched {

    namespace init {
        void setup();
        void insertTask(Task* task);
    }

    bool isEnabled();

    /* don't do this, ever, after booting (unless
     * you are the mutex implementation etc.): */
    void enable();
    void disable();

    void yield();

    Task* currentTask();
    UserTask* currentUserTask();
    void insertTask(Task* task);

}
