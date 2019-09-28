#include "write.h"
#include "send_que.h"
#include "struct.h"
#include "config_mgr.h"
#include "period_msg.h"
#include "statistics.h"
#include "reg.h"

#define LOG_TAG "send"

// 在此声明内部函数...
static int _WriteCmd(const FpgaRegBaseAddr* reg, const Fh228Priority priority, const int msgNo);
static int _WriteData(DevContext *dc, const uint32_t subAddr, const char *buf, const int bytes, ChnProp* chn);
static int _WriteDataBySmallPackage(
    ChnProp* chn,
    FpgaRegBaseAddr *reg,
    SendQueContext* sendQue,
    const char *buf,
    const int bytes
);

// 将数据写入DMA节点, 通知FPGA
static bool _WriteDma2Fpga(
    FpgaRegBaseAddr *reg,
    int msgNo,
    SendQueContext* sendQue,
    const int priority,
    const char *buf,
    const int dataTotalBytes,
    const int payloadBytes,
    int sop,
    int eop
);

// 将数据写入DMA节点, 通知FPGA, 加了重试
static bool _WriteDma2FpgaWithRetry(
    FpgaRegBaseAddr *reg,
    int msgNo,
    SendQueContext* sendQue,
    const int priority,
    const char *buf,
    const int dataTotalBytes,
    const int payloadBytes,
    int sop,
    int eop
);

// 将数据切成小包, 逐一顺序写到DMA节点中
static int _WriteDataBySmallPackage(
    ChnProp* chn,
    FpgaRegBaseAddr *reg,
    SendQueContext* sendQue,
    const char *buf,
    const int bytes
);

//////////////////////////////////////////////////////////////////////////

int Write_SendDataOrCmd(DevContext *dc, const uint32_t subAddr, const char *buf, const int bytes)
{
    ChnProp* chn = CfgMgr_GetChnProp(&dc->cfgCtx, subAddr); // 根据子地址找到对应的发送通道
    if (!chn)
    {
        elog_e(LOG_TAG, "channel not found, sa=%u", subAddr);
        return -1;
    }

    if (CfgMgr_IsChnTx(chn)) // 发送数据
    {
        return _WriteData(dc, subAddr, buf, bytes, chn);
    }
    else // 发送命令
    {
        Statistics_AddNcCmdCount(chn->msgNo, chn->s_priority, 1);
        Statistics_AddNcCmdBytes(chn->msgNo, chn->s_priority, chn->s_dataByteCount);
        return _WriteCmd(dc->reg, chn->s_priority, chn->msgNo);
    }
}

bool Write_SetPreplanData(DevContext *dc, const uint32_t subAddr, const char *buf, const uint32_t bytes, const bool bForce)
{
    // 根据子地址找到对应的消息通道, 再得到通道所对应的乒乓buf
    ChnProp* chn = CfgMgr_GetChnProp(&dc->cfgCtx, subAddr);
    CsPingPongBufContext* pingpong = &chn->periodMsg;
    SendQueNode* node = NULL;
    if (!bForce) // 非强制的话, 要判断下上一个预配数据有没有被FPGA发送过, 如果没有, 则不能预配
    {
        node = (SendQueNode*)Cs_PingPongBufGetBuf(pingpong);
        if (false == SendQue_IsNodeWriteable(node))
        {
            elog_w(LOG_TAG, "sorry, old msg is not sent by fpga, so your need to wait");
            return false;
        }
    }

    Cs_PingPongBufSwap(pingpong); // 切换乒乓BUG
    node = (SendQueNode*)Cs_PingPongBufGetBuf(pingpong);
    if (bytes > 0)
    {
        memcpy(node->dmaBuf->data, buf, bytes);
    }

    SendQue_SetNodeNonWriteable(node, bytes, bytes, 1, 1);
    Reg_PushChannelDmaBuf2Fpga(dc->reg, chn, sizeof(W0) + sizeof(W1) + bytes, node->index);

    // TODO: 什么时候激活周期消息, 有待确认
    if (CfgMgr_IsChnRx(chn))
    {
        PeriodMsg_SetFpgaEnable(dc->reg, chn->msgNo, true);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

static int _WriteCmd(const FpgaRegBaseAddr* reg, const Fh228Priority priority, const int msgNo)
{
    Reg_NcSendCmd(reg, priority, msgNo);
    return 0;
}

static int _WriteData(
    DevContext *dc,
    const uint32_t subAddr,
    const char *buf,
    const int bytes,
    ChnProp* chn
)
{
    SendQueContext* que = NULL;

    if (bytes != chn->s_dataByteCount) // 发送方向的话, 数据长度必须和配置中的长度, 保持一致
    {
        elog_e(LOG_TAG, "sendLen is not equal to channelLen, sendLen=%d, channelLen=%u", bytes, chn->s_dataByteCount);
        return -1;
    }

    if (bytes > SendQueMgr_GetValidDmaCapability(chn->s_priority))
    {
        elog_e(LOG_TAG, "sendLen[=%d] is too big, for priority[=%d], maxBytes is [=%d]",
            bytes, chn->s_priority, SendQueMgr_GetValidDmaCapability(chn->s_priority));
        return -1;
    }

    // 根据通道信息, 找到对应的发送队列
    que = SendQueMgr_Get(&dc->sendQueMgr, chn->s_priority);
    if (!que)
    {
        elog_e(LOG_TAG, "sendQue not found, priority=%d", chn->s_priority);
        return -1;
    }

    // 将用户数据, 切成小包 , 往发送队列里送
    return _WriteDataBySmallPackage(chn, dc->reg, que, buf, bytes);
}

static bool _WriteDma2Fpga(
    FpgaRegBaseAddr *reg,
    int msgNo,
    SendQueContext* sendQue,
    const int priority,
    const char *buf,
    const int dataTotalBytes,
    const int payloadBytes,
    int sop,
    int eop)
{
    bool suc = false;
    SendQueNode* node = NULL;

    SendQue_Lock(sendQue); // 加锁
    // 读取下链表的头节点, 看看是不是可以往里面写数据
    node = SendQue_PeekFront(sendQue);
    if (!SendQue_IsNodeWriteable(node))
    {
        goto Exit0;
    }

    if (payloadBytes > 0)
    {
        memcpy(node->dmaBuf->data, buf, payloadBytes); // 把用户要发送的数据, 写到节点的DMA里
    }

    // 将这个节点标记为不可写. 供FPGA使用, FPGA使用完, 会重新将它标记为可写
    SendQue_SetNodeNonWriteable(node, dataTotalBytes, payloadBytes, sop, eop);

    // 将节点告诉FPGA
    Reg_PushDmaBuf2Fpga(reg, node->index, sizeof(W0) + sizeof(W1) + payloadBytes, msgNo, eop, priority);
    SendQue_Move2NextNode(sendQue); // 移动到下一个节点
    Statistics_AddSendBytes(msgNo, priority, payloadBytes); // 统计

    suc = true;

Exit0:
    SendQue_Unlock(sendQue); // 解锁
    return suc;
}

static bool _WriteDma2FpgaWithRetry(
    FpgaRegBaseAddr *reg,
    int msgNo,
    SendQueContext* sendQue,
    const int priority,
    const char *buf,
    const int dataTotalBytes,
    const int payloadBytes,
    int sop,
    int eop)
{
    int i = 0;
    do
    {
        if (_WriteDma2Fpga(reg, msgNo, sendQue, priority, buf, dataTotalBytes, payloadBytes, sop, eop))
        {
            return true;
        }
        Ba_TaskSleep(0);
        i++;
    } while (i < 1000);

    elog_e(LOG_TAG, "failed to send dma buf to fpga, retry count = %d", i + 1);
    return false;
}

/**
 * @brief 将用户数据, 切成小包, 写入DMA节点中, 并标记好sop和eop
 * @return 成功发送的数据大小
 * @attention 返回值是被发送出去的大小, 如果不等于数据大小, 则认识发送失败
 */
static int _WriteDataBySmallPackage(
    ChnProp* chn,
    FpgaRegBaseAddr *reg,
    SendQueContext* sendQue,
    const char *buf,
    const int bytes
)
{
    bool isFirst      = true; // 是不是第一包
    bool suc          = false;
    int sentSum       = 0; // 已发送的大小
    int sop           = 0;
    int eop           = 0;
    int payloadBytes  = 0;
    int validDmaBytes = sendQueGetDmaBufSize(sendQue) - sizeof(W0) - sizeof(W1);
    char* payloadBuf  = NULL;
    CsFastBufSpliterContext spliter;

    Cs_FastBufSplitterAttach(&spliter, buf, bytes);

    do
    {
        payloadBytes = min(bytes - sentSum, validDmaBytes); // 拿剩余数据的大小, 和DMA有效的大小比较, 取最小的
        payloadBuf = Cs_FastBufSplitterPop(&spliter, payloadBytes); // 从DMA中切出一段可容纳下paload的buf
        eop              = ((sentSum + payloadBytes) >= bytes) ? 1 : 0;
        sop              = isFirst ? 1 : 0;

        suc = _WriteDma2FpgaWithRetry(reg, chn->msgNo, sendQue, chn->s_priority, payloadBuf, bytes, payloadBytes, sop, eop);
        if (!suc)
        {
            elog_e(LOG_TAG, "send failed, dataTotalBytes=%d, sendBytes=%d", bytes, sentSum);
            break;
        }

        if (isFirst)
        {
            isFirst = false;
        }
        sentSum += payloadBytes;
    } while (sentSum < bytes);

    if (sentSum == bytes) // 发送成功, 加个统计
    {
        Statistics_AddSendPackage(chn->msgNo, chn->s_priority, 1);
    }

    return sentSum;
}
