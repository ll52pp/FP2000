#include "recv_que_mgr.h"
#include "struct.h"
#include "fpga_reg_addr.h"
#include "utils.h"
#include "reg.h"
#include "dma_info.h"

#define LOG_TAG "recvQueMgr"

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t buf_num_of_1st : 10;/* [ 0: 9] : RW buffer number     -- 1st available buffer number*/
    uint32_t buf_num_of_2nd : 10;/* [10:19] : RW buffer number     -- 2nd available buffer number*/
    uint32_t buf_num_of_3rd : 10;/* [20:29] : RW buffer number     -- 3rd available buffer number*/
    uint32_t valid_buf_num : 2; /* [30:31] : RW valid number of buffers*/
#else
    uint32_t valid_buf_num : 2; /* [30:31] : RW valid number of buffers*/
    uint32_t buf_num_of_3rd : 10;/* [20:29] : RW buffer number     -- 3rd available buffer number*/
    uint32_t buf_num_of_2nd : 10;/* [10:19] : RW buffer number     -- 2nd available buffer number*/
    uint32_t buf_num_of_1st : 10;/* [ 0: 9] : RW buffer number     -- 1st available buffer number*/
#endif
} Credit;

// 内部函数在此声明
static void RecvQueMgr_UninitQue(RecvQueContext* que);
static void RecvQueMgr_Reg2Fpga(FpgaRegBaseAddr* reg, Fh228Priority priority, RecvQueContext* context);
static void RecvQueMgr_SetRxBufSize2Fgpa(FpgaRegBaseAddr *reg, Fh228Priority priority, int size);
static void RecvQueMgr_PushOneCredit2Fpga(FpgaRegBaseAddr* reg, Fh228Priority priority, int nodeIndex);
static void RecvQueMgr_RegLa2Fpga(FpgaRegBaseAddr *reg, Fh228Priority priority, int index, unsigned long la);
static void RecvQueMgr_InitQue(
    FpgaRegBaseAddr* reg,
    Fh228Priority priority,
    RecvQueContext* que,
    int dmaNodeCnt,
    int dmaNodeSize
);

void RecvQueMgr_Init(RecvQueMgrContext* ctx, FpgaRegBaseAddr* reg)
{
    ctx->reg = reg;
    RecvQueMgr_InitQue(reg, FH228_PRIORITY_LOW, &ctx->arrRecvQue[FH228_PRIORITY_LOW], RX_NODE_CNT_LOW, RX_NODE_SIZE_LOW);
    RecvQueMgr_InitQue(reg, FH228_PRIORITY_MID, &ctx->arrRecvQue[FH228_PRIORITY_MID], RX_NODE_CNT_MID, RX_NODE_SIZE_MID);
    RecvQueMgr_InitQue(reg, FH228_PRIORITY_HIGH, &ctx->arrRecvQue[FH228_PRIORITY_HIGH], RX_NODE_CNT_HIGH, RX_NODE_SIZE_HIGH);
}

void RecvQueMgr_Uninit(RecvQueMgrContext* ctx)
{
    RecvQueMgr_UninitQue(&ctx->arrRecvQue[FH228_PRIORITY_LOW]);
    RecvQueMgr_UninitQue(&ctx->arrRecvQue[FH228_PRIORITY_MID]);
    RecvQueMgr_UninitQue(&ctx->arrRecvQue[FH228_PRIORITY_HIGH]);
    ctx->reg = NULL;
}

RecvQueContext* RecvQueMgr_Get(RecvQueMgrContext* ctx, Fh228Priority priority)
{
    if ((priority >= FH228_PRIORITY_LOW) && (priority <= FH228_PRIORITY_HIGH))
    {
        return &ctx->arrRecvQue[priority];
    }

    return NULL;
}

void RecvQueMgr_RecycleNode(RecvQueMgrContext* ctx, RecvQueNode* node, Fh228Priority priority)
{
    RecvQue_SetNodeEmpty(node);
    RecvQue_PushBack(&ctx->arrRecvQue[priority], node);
    RecvQueMgr_PushOneCredit2Fpga(ctx->reg, priority, node->index);
}

void RecvQueMgr_InitQue(
    FpgaRegBaseAddr* reg,
    Fh228Priority priority,
    RecvQueContext* que,
    int dmaNodeCnt,
    int dmaNodeSize
)
{
    RecvQue_Init(que, dmaNodeCnt, dmaNodeSize, 0);
    RecvQueMgr_Reg2Fpga(reg, priority, que);
    RecvQueMgr_SetRxBufSize2Fgpa(reg, priority, RecvQue_GetDmaBufSize(que));
}

void RecvQueMgr_UninitQue(RecvQueContext* que)
{
    RecvQue_Uninit(que);
}

void RecvQueMgr_RegLa2Fpga(FpgaRegBaseAddr *reg, Fh228Priority priority, int index, unsigned long la)
{
    uint32_t addr = 0;

    switch (priority)
    {
    case FH228_PRIORITY_LOW:
        addr = FPGA_REG_ADDR_RX_BUF_LA_LOW;
        break;
    case FH228_PRIORITY_MID:
        addr = FPGA_REG_ADDR_RX_BUF_LA_MID;
        break;
    case FH228_PRIORITY_HIGH:
        addr = FPGA_REG_ADDR_RX_BUF_LA_HIGH;
        break;
    default:
        elog_e(LOG_TAG, "priority not found");
        return;
    }

    Reg_WriteIndirectAddr32(reg, addr + index, 0, la);
}

void RecvQueMgr_Reg2Fpga(FpgaRegBaseAddr* reg, Fh228Priority priority, RecvQueContext* context)
{
    RecvQueNode* next = (RecvQueNode*)Cs_RingBufPeekFront(&context->ringList);
    while (next)
    {
        RecvQueMgr_RegLa2Fpga(reg, priority, next->index, (unsigned long)next->dmaBuf);
        RecvQueMgr_PushOneCredit2Fpga(reg, priority, next->index);
        next = (RecvQueNode*)Cs_RingBufNext(next);
    }
}

void RecvQueMgr_SetRxBufSize2Fgpa(FpgaRegBaseAddr *reg, Fh228Priority priority, int size)
{
    uint32_t addr = 0;

    switch (priority)
    {
    case FH228_PRIORITY_LOW:
        addr = FPGA_REG_ADDR_RX_BUF_SIZE_LOW;
        break;
    case FH228_PRIORITY_MID:
        addr = FPGA_REG_ADDR_RX_BUF_SIZE_MID;
        break;
    case FH228_PRIORITY_HIGH:
        addr = FPGA_REG_ADDR_RX_BUF_SIZE_HIGH;
        break;
    default:
        elog_e(LOG_TAG, "priority not found");
        return;
    }

    Reg_WriteIndirectAddr32(reg, addr, 0, size / 4096);
}

void RecvQueMgr_PushOneCredit2Fpga(FpgaRegBaseAddr* reg, Fh228Priority priority, int nodeIndex)
{
    Credit credit;
    ZeroStruct(credit);
    credit.buf_num_of_1st = nodeIndex;
    credit.buf_num_of_2nd = 0;
    credit.buf_num_of_3rd = 0;
    credit.valid_buf_num = 1;

    switch (priority)
    {
    case FH228_PRIORITY_LOW:
        Ba_RegisterWrite32Struct(&reg->credit0, credit);
        break;
    case FH228_PRIORITY_MID:
        Ba_RegisterWrite32Struct(&reg->credit1, credit);
        break;
    case FH228_PRIORITY_HIGH:
        Ba_RegisterWrite32Struct(&reg->credit2, credit);
        break;
    default:
        elog_e(LOG_TAG, "priority not found, priority=%d", priority);
        break;
    }
}
