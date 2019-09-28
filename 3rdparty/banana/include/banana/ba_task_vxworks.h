#ifndef BA_TASK_VXWORKS_H_D10C2ECC_CC47_4A2B_8282_164AB3CA6566
#define BA_TASK_VXWORKS_H_D10C2ECC_CC47_4A2B_8282_164AB3CA6566

#include <vxworks.h>
#include <tasklib.h>
#include <sysLib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int(*Ba_TaskProcType)(void* param);

#define BaTaskContext int

static int Ba_TaskCreate(BaTaskContext* ctx, int priority, int stackSize, Ba_TaskProcType proc, void* param)
{
    *ctx = taskSpawn(NULL, priority, VX_FP_TASK, stackSize, (FUNCPTR)proc, (int)param, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (ERROR != *ctx)
    {
        return 0;
    }

    return -1;
}

static void Ba_TaskDelete(BaTaskContext* ctx)
{
    taskDelete(*ctx);
}

static void Ba_TaskSleep(unsigned int milliseconds)
{
    int m = sysClkRateGet();
    m = 1000 / m;
    m = milliseconds / m + 1;
    taskDelay(m);
}

#ifdef __cplusplus
}
#endif

#endif
