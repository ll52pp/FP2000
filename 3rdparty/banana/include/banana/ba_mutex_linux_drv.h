#ifndef BA_MUTEX_H_LINUX_DRV_H_9E41EAA3_75F7_407C_9DC0_AE4B04D3BA5C
#define BA_MUTEX_H_LINUX_DRV_H_9E41EAA3_75F7_407C_9DC0_AE4B04D3BA5C

#include <linux/mutex.h>
#include "ba_base_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mutex BaMutexContext;

static bool Ba_MutexInit(BaMutexContext* context)
{
    mutex_init(context);
    return true;
}

static void Ba_MutexUninit(BaMutexContext* context)
{
}

static void Ba_MutexLock(BaMutexContext* context)
{
    mutex_lock(context);
}

static void Ba_MutexUnlock(BaMutexContext* context)
{
    mutex_unlock(context);
}

#ifdef __cplusplus
}
#endif

#endif
