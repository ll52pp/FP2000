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
    // ��ΪKeDelayExecutionThread����100����Ϊ��С����, ������Ҫת���ɺ���Ϊ��λ
    // KeDelayExecutionThread�Ĳ���ֵΪ����, ��ʾ�ǵľ���ʱ��, Ϊ��ʱ��ʾ�������ʱ��

    LARGE_INTEGER val;
    val.QuadPart = -10 * 1000; // ת����1����, ��ʱ��ʾ�������ʱ��
    val.QuadPart *= milliseconds;
    KeDelayExecutionThread(KernelMode, FALSE, &val);
}

#ifdef __cplusplus
}
#endif
