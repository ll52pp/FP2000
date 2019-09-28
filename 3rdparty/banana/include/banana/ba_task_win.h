#pragma once

#include <windows.h>
#include <process.h>

#if _MSC_VER <= 1900
typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef int(*Ba_TaskProcType)(void* param);

#define BaTaskContext HANDLE

static int Ba_TaskCreate(BaTaskContext* ctx, int priority, int stackSize, Ba_TaskProcType proc, void* param)
{
    BaTaskContext handle = (BaTaskContext)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)proc, param, 0, NULL);
    if (handle)
    {
        CloseHandle(handle);
        return 0;
    }

    return -1;
}

static void Ba_TaskDelete(BaTaskContext* ctx)
{
}

static void Ba_TaskSleep(unsigned int milliseconds)
{
    Sleep(milliseconds);
}

#ifdef __cplusplus
}
#endif
