// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <task.h>
#include <mm.h>
#include <lib/string.h>
#include <context.h>
#include <arch/asm.h>
#include <debug.h>
#include <scheduler.h>
#include <syscalls.h>

typedef enum
{
    SCHED_TASK_RUNNING,
    SCHED_TASK_SLEEPING,
    SCHED_TASK_KILLED
} sched_task_state_t;

// id, running/sleeping/killed
typedef struct {
    size_t tid;
    int state;
    int cpu_usage;
} sched_task_info_t;

namespace sched {
    extern lib::List<Task*> s_taskList;
}

ssize_t get_sched_task_info(size_t tid, void* info_ptr)
{
    auto info = (sched_task_info_t*)info_ptr;
    lib::ListItem<Task*>* found = nullptr;
    for (auto it = sched::s_taskList.begin();
         it != nullptr;
         it = it->next)
    {
        auto task = it->item;
        if (task->tid() == tid) {
            found = it;
            break;
        }
    }

    if (found == nullptr)
        found = sched::s_taskList.begin();

    info->state = found->item->state();
    info->tid = found->item->tid();

    if (found->next)
        return found->next->item->tid();
    return -1;
}
