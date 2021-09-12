// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: MIT

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <sys/types.h>

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

ssize_t get_sched_task_info(size_t tid, sched_task_info_t* info);

void yield();

#endif // SCHEDULER_H
