#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <wdm.h>

typedef int(*Ba_TaskProcType)(void* param);

typedef struct
{
    PKTHREAD _pkthd;
} BaTaskContext;

static int Ba_TaskCreate(BaTaskContext* ctx, int priority, int stackSize, Ba_TaskProcType proc, void* param)
{
    HANDLE handle = NULL;
    NTSTATUS status = PsCreateSystemThread(&handle, THREAD_ALL_ACCESS, NULL, NULL, NULL, proc, param);
    if (NT_SUCCESS(status))
    {
        ObReferenceObjectByHandle(handle, THREAD_ALL_ACCESS, NULL, KernelMode, &ctx->_pkthd, NULL);
        ZwClose(handle);
        return 0;
    }

    return -1;
}

static int Ba_TaskDelete(BaTaskContext* ctx)
{
}

static void Ba_TaskSleep(unsigned int milliseconds)
{
    // 因为KeDelayExecutionThread是以100纳秒为最小粒度, 所以需要转换成毫秒为单位
    // KeDelayExecutionThread的参数值为正是, 表示是的绝对时间, 为负时表示的是相对时间

    LARGE_INTEGER val;
    val.QuadPart = -10 * 1000; // 转换成1毫秒, 负时表示的是相对时间
    val.QuadPart *= milliseconds;
    KeDelayExecutionThread(KernelMode, FALSE, &val);
}

#ifdef __cplusplus
}
#endif
