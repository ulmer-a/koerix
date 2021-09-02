#include <scheduler.h>
#include <asm.h>

static bool s_schedEnable = false;

namespace sched {

void yield()
{
    arch_yield();
}

namespace init {
    void enable()
    {
        s_schedEnable = true;
    }
}

void schedule()
{

}

}
