#pragma once

class Task;

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
    void insertTask(Task* task);

}
