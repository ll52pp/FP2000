#ifndef SEND_QUE_MGR_H
#define SEND_QUE_MGR_H

#include "common_header.h"
#include "send_que.h"

typedef struct
{
    SendQueContext arrSendQue[FH228_PRIORITY_COUNT]; ///< ���������ȼ���Ϣ����
    SendQueContext preplanMsgQue; ///< Ԥ����Ϣ����
} SendQueMgrContext;

void SendQueMgr_Init(SendQueMgrContext* ctx, FpgaRegBaseAddr* reg);
void SendQueMgr_Uninit(SendQueMgrContext* ctx);

SendQueContext* SendQueMgr_Get(SendQueMgrContext* ctx, Fh228Priority priority);
SendQueContext* SendQueMgr_GetPreplanMsgQue(SendQueMgrContext* ctx);

// ��ȡ����DMA�ڵ��������(�����нڵ��ۼ�������������, ��ȥ��W0W1�����ı��ռ�õĴ�С)
int SendQueMgr_GetValidDmaCapability(Fh228Priority priority);

#endif
