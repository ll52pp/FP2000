#ifndef BA_MUTEX_H_VXWORKS_H_9E41EAA3_75F7_407C_9DC0_AE4B04D3BA5C
#define BA_MUTEX_H_VXWORKS_H_9E41EAA3_75F7_407C_9DC0_AE4B04D3BA5C

#include <vxworks.h>
#include <semLib.h>
#include "ba_base_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef SEM_ID BaMutexContext;

#define _BA_MUTEXT_VXWORKS_CAST(x) (*((SEM_ID*)(x)))

static bool Ba_MutexInit(BaMutexContext* context)
{
    _BA_MUTEXT_VXWORKS_CAST(context) = semMCreate(SEM_Q_PRIORITY);
    if (NULL != _BA_MUTEXT_VXWORKS_CAST(context))
    {
        return true;
    }

    return false;
}

static void Ba_MutexUninit(BaMutexContext* context)
{
    semDelete(_BA_MUTEXT_VXWORKS_CAST(context));
}

static void Ba_MutexLock(BaMutexContext* context)
{
    semTake(_BA_MUTEXT_VXWORKS_CAST(context), WAIT_FOREVER);
}

static void Ba_MutexUnlock(BaMutexContext* context)
{
    semGive(_BA_MUTEXT_VXWORKS_CAST(context));
}

#ifdef __cplusplus
}
#endif

#endif
