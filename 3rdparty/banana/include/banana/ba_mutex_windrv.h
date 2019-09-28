#pragma once
#include <wdf.h>
#include <ntintsafe.h>
#include <wdftypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef WDFSPINLOCK BaMutexContext;

static bool Ba_MutexInit(BaMutexContext* context)
{
    NTSTATUS status;
    WDF_OBJECT_ATTRIBUTES attributes;
    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    status = WdfSpinLockCreate(&attributes, (WDFSPINLOCK*)context);
    return NT_SUCCESS(status);
}

static void Ba_MutexUninit(BaMutexContext* context)
{
}

static void Ba_MutexLock(BaMutexContext* context)
{
    WdfSpinLockAcquire(*((WDFSPINLOCK*)context));
}

static void Ba_MutexUnlock(BaMutexContext* context)
{
    WdfSpinLockRelease(*((WDFSPINLOCK*)context));
}

#ifdef __cplusplus
}
#endif
