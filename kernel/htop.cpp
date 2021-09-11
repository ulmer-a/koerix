#include <task.h>
#include <mm.h>
#include <string.h>
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
    int state;
    int cpu_usage;
} sched_task_info_t;

ssize_t get_sched_task_info(size_t tid, void* info_ptr)
{
    auto info = (sched_task_info_t*)info_ptr;
    if (tid == 0)
    {
        info->state = SCHED_TASK_RUNNING;
        info->cpu_usage = 50;
        return 1;
    }
    else
    {
        info->state = SCHED_TASK_KILLED;
        info->cpu_usage = 0;
        return -1;
    }
}




