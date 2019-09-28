#include "ut_header.h"

int myTaskProc(void* param)
{
    return 0;
}

void Cs_TestTask(void **state)
{
    BaTaskContext ctx;
    Ba_TaskCreate(&ctx, 0, 0, myTaskProc, (void*)10);
}
