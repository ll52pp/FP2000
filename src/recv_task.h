#ifndef FH228_RECV_TASK_H
#define FH228_RECV_TASK_H

#include "common_header.h"
#include "config_mgr.h"
#include "recv_que_mgr.h"

typedef struct
{
    BaTaskContext task;
    CfgMgrCtx* cfg;
    RecvQueMgrContext* recvQueMgr;
    bool isNeedExit; ///< �Ƿ��˳��߳�
    bool isNeedWork; ///< �Ƿ���Ҫ�߳�����
} RecvTaskCtx;

bool RecvTask_Init(RecvTaskCtx* ctx, CfgMgrCtx* cfg, RecvQueMgrContext *recvQueMgr);
void RecvTask_Start(RecvTaskCtx* ctx);
void RecvTask_Pause(RecvTaskCtx* ctx);
void RecvTask_Uninit(RecvTaskCtx* ctx);

#endif
