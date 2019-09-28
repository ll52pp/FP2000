#include "recv_task.h"
#include "read.h"

#define LOG_TAG "recv_task"

static int  RecvTask_Entry(void* param);
static void RecvTask_DoWork(CfgMgrCtx* cfg, RecvQueMgrContext *recvQueMgr);

bool RecvTask_Init(RecvTaskCtx* ctx, CfgMgrCtx* cfg, RecvQueMgrContext *recvQueMgr)
{
    const int priority = 50;
    const int stackSize = 1048576;

    ctx->cfg = cfg;
    ctx->recvQueMgr = recvQueMgr;
    ctx->isNeedExit = false;
    ctx->isNeedWork = false;

    if (0 == Ba_TaskCreate(&ctx->task, priority, stackSize, (Ba_TaskProcType)RecvTask_Entry, (void*)ctx))
    {
        return true;
    }

    elog_e(LOG_TAG, "failed create task");
    return false;
}

void RecvTask_Start(RecvTaskCtx* ctx)
{
    ctx->isNeedWork = true;
}

void RecvTask_Pause(RecvTaskCtx* ctx)
{
    ctx->isNeedWork = false;
}

void RecvTask_Uninit(RecvTaskCtx* ctx)
{
    ctx->isNeedWork = false;
    ctx->isNeedExit = true;
}

static int RecvTask_Entry(void* param)
{
    RecvTaskCtx *theTask = (RecvTaskCtx *)param;

    while (false == theTask->isNeedExit)
    {
        if (theTask->isNeedWork)
        {
            RecvTask_DoWork(theTask->cfg, theTask->recvQueMgr);
        }
        else
        {
            Ba_TaskSleep(10);
        }
    }

    elog_v(LOG_TAG, "succed to exit recv task");
    return 0;
}

void RecvTask_DoWork(CfgMgrCtx* cfg, RecvQueMgrContext *recvQueMgr)
{
    if (Read_FromRecvQue(cfg, recvQueMgr, FH228_PRIORITY_HIGH))
    {
        return;
    }

    if (Read_FromRecvQue(cfg, recvQueMgr, FH228_PRIORITY_LOW))
    {
        return;
    }

    Ba_TaskSleep(1); // 若本次没有数据可读, sleep一下
}
