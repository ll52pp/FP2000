#include "send_que_mgr.h"
#include "struct.h"
#include "utils.h"
#include "fpga_reg_addr.h"
#include "reg.h"
#include "dma_info.h"

#define LOG_TAG "sendQueMgr"

// 内部函数在此声明...
static void _InitQue(FpgaRegBaseAddr* reg, SendQueContext* que, int dmaNodeCnt, int dmaNodeSize, int msgNumBase);
static void _UninitQue(SendQueContext* que);
static void _RegDma2Fpga(FpgaRegBaseAddr* reg, SendQueContext* context);

void SendQueMgr_Init(SendQueMgrContext* ctx, FpgaRegBaseAddr* reg)
{
    _InitQue(reg, &ctx->arrSendQue[FH228_PRIORITY_HIGH], TX_NODE_CNT_HIGH, TX_NODE_SIZE_HIGH, TX_NODE_START_INDEX_HIGH);
    _InitQue(reg, &ctx->arrSendQue[FH228_PRIORITY_MID], TX_NODE_CNT_MID, TX_NODE_SIZE_MID, TX_NODE_START_INDEX_MID);
    _InitQue(reg, &ctx->arrSendQue[FH228_PRIORITY_LOW], TX_NODE_CNT_LOW, TX_NODE_SIZE_LOW, TX_NODE_START_INDEX_LOW);
    _InitQue(reg, &ctx->preplanMsgQue, TX_NODE_CNT_PREPLAN, TX_NODE_SIZE_PREPLAN, TX_NODE_START_INDEX_PREPLAN);
}

void SendQueMgr_Uninit(SendQueMgrContext* ctx)
{
    _UninitQue(&ctx->arrSendQue[FH228_PRIORITY_HIGH]);
    _UninitQue(&ctx->arrSendQue[FH228_PRIORITY_MID]);
    _UninitQue(&ctx->arrSendQue[FH228_PRIORITY_LOW]);
    _UninitQue(&ctx->preplanMsgQue);
}

SendQueContext* SendQueMgr_Get(SendQueMgrContext* ctx, Fh228Priority priority)
{
    if ((priority >= FH228_PRIORITY_LOW) && (priority <= FH228_PRIORITY_HIGH))
    {
        return &ctx->arrSendQue[priority];
    }

    return NULL;
}

SendQueContext* SendQueMgr_GetPreplanMsgQue(SendQueMgrContext* ctx)
{
    return &ctx->preplanMsgQue;
}

int SendQueMgr_GetValidDmaCapability(Fh228Priority priority)
{
    switch (priority)
    {
    case FH228_PRIORITY_LOW:
        return MAX_SEND_BUF_SIZE_PRIORITY_LOW;
    case FH228_PRIORITY_HIGH:
        return MAX_SEND_BUF_SIZE_PRIORITY_HIGH;
    default:
        return 0;
    }
}

void _InitQue(FpgaRegBaseAddr* reg, SendQueContext* que, int dmaNodeCnt, int dmaNodeSize, int msgNumBase)
{
    SendQue_Init(que, dmaNodeCnt, dmaNodeSize, msgNumBase);
    _RegDma2Fpga(reg, que);
}

void _UninitQue(SendQueContext* que)
{
    SendQue_Uninit(que);
}

void _RegDma2Fpga(FpgaRegBaseAddr* reg, SendQueContext* context)
{
    SendQueNode* next = (SendQueNode*)Cs_RingBufPeekFront(&context->ringList);
    while (next)
    {
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_LA + next->index, 1, (unsigned int)next->dmaBuf);
        next = (SendQueNode*)Cs_RingBufNext(next);
    }
}
