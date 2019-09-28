#pragma once
#include <windows.h>
#include "ba_base_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef CRITICAL_SECTION BaMutexContext;

static bool Ba_MutexInit(BaMutexContext* context)
{
    InitializeCriticalSection(context);
    return true;
}

static void Ba_MutexUninit(BaMutexContext* context)
{
    DeleteCriticalSection(context);
}

static void Ba_MutexLock(BaMutexContext* context)
{
    EnterCriticalSection(context);
}

static void Ba_MutexUnlock(BaMutexContext* context)
{
    LeaveCriticalSection(context);
}

#ifdef __cplusplus
}
#endif
