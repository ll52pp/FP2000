#ifndef BA_RWLOCK_H_0C04993E_820F_46E0_9588_CD262C1D9271
#define BA_RWLOCK_H_0C04993E_820F_46E0_9588_CD262C1D9271

#include "ba_base_types.h"
#include "ba_mutex.h"

typedef struct
{
    BaMutexContext rlock;
    BaMutexContext wlock;
    int rCount; // ¶ÁÕß¸öÊý
} BaRwLockContext;

static bool Ba_RwLockInit(BaRwLockContext* ctx)
{
    ctx->rCount = 0;
    Ba_MutexInit(&ctx->rlock);
    Ba_MutexInit(&ctx->wlock);
}

static bool Ba_RwLockUninit(BaRwLockContext* ctx)
{
    Ba_MutexUninit(&ctx->rlock);
    Ba_MutexUninit(&ctx->wlock);
    ctx->rCount = 0;
}

static void Ba_RwReadLock(BaRwLockContext* ctx)
{
    Ba_MutexLock(&ctx->rlock);
    ctx->rCount++;
    if (ctx->rCount == 1)
    {
        Ba_MutexLock(&ctx->wlock);
    }
    Ba_MutexUnlock(&ctx->rlock);
}

static void Ba_RwReadUnlock(BaRwLockContext* ctx)
{
    Ba_MutexLock(&ctx->rlock);
    ctx->rCount--;
    if (ctx->rCount == 0)
    {
        Ba_MutexUnlock(&ctx->wlock);
    }
    Ba_MutexUnlock(&ctx->rlock);
}

static void Ba_RwWriteLock(BaRwLockContext* ctx)
{
    Ba_MutexLock(&ctx->wlock);
}

static void Ba_RwWriteUnLock(BaRwLockContext* ctx)
{
    Ba_MutexUnlock(&ctx->wlock);
}

#endif
