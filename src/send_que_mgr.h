#ifndef SEND_QUE_MGR_H
#define SEND_QUE_MGR_H

#include "common_header.h"
#include "send_que.h"

typedef struct
{
    SendQueContext arrSendQue[FH228_PRIORITY_COUNT]; ///< 正常的优先级消息队列
    SendQueContext preplanMsgQue; ///< 预配消息队列
} SendQueMgrContext;

void SendQueMgr_Init(SendQueMgrContext* ctx, FpgaRegBaseAddr* reg);
void SendQueMgr_Uninit(SendQueMgrContext* ctx);

SendQueContext* SendQueMgr_Get(SendQueMgrContext* ctx, Fh228Priority priority);
SendQueContext* SendQueMgr_GetPreplanMsgQue(SendQueMgrContext* ctx);

// 获取所有DMA节点的总容量(即所有节点累加起来的总容量, 减去了W0W1这样的标记占用的大小)
int SendQueMgr_GetValidDmaCapability(Fh228Priority priority);

#endif
