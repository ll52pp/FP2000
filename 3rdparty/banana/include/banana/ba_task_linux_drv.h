#ifndef BA_TASK_LINUX_DRV_H_D10C2ECC_CC47_4A2B_8282_164AB3CA6566
#define BA_TASK_LINUX_DRV_H_D10C2ECC_CC47_4A2B_8282_164AB3CA6566

#include <linux/kthread.h>
#include <linux/delay.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int(*Ba_TaskProcType)(void* param);

typedef struct task_struct BaTaskContext;

static int Ba_TaskCreate(BaTaskContext* ctx, int priority, int stackSize, Ba_TaskProcType proc, void* param)
{
    ctx = kthread_run(proc, param, "ba_task_name");
    if (IS_ERR(ctx))
    {
        return -1;
    }
    return 0;
}

static void Ba_TaskDelete(BaTaskContext* ctx)
{
}

static void Ba_TaskSleep(unsigned int milliseconds)
{
    mdelay(milliseconds);
}

#ifdef __cplusplus
}
#endif

#endif
