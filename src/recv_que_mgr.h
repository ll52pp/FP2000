#ifndef RECV_QUE_MGR_H
#define RECV_QUE_MGR_H

#include "common_header.h"
#include "recv_que.h"

typedef struct
{
    RecvQueContext arrRecvQue[FH228_PRIORITY_COUNT];
    FpgaRegBaseAddr* reg;
} RecvQueMgrContext;

void RecvQueMgr_Init(RecvQueMgrContext* ctx, FpgaRegBaseAddr* reg);
void RecvQueMgr_Uninit(RecvQueMgrContext* ctx);
void RecvQueMgr_RecycleNode(RecvQueMgrContext* ctx, RecvQueNode* node, Fh228Priority priority);
RecvQueContext* RecvQueMgr_Get(RecvQueMgrContext* ctx, Fh228Priority priority);

#endif
