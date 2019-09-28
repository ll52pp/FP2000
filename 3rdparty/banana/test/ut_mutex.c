#include "ut_header.h"

void Cs_TestMutex(void **state)
{
    BaMutexContext ctx;
    Ba_MutexInit(&ctx);
    Ba_MutexLock(&ctx);
    Ba_MutexUnlock(&ctx);
    Ba_MutexUninit(&ctx);
}
