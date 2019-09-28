#include "write.h"
#include "send_que.h"
#include "struct.h"
#include "config_mgr.h"
#include "period_msg.h"
#include "statistics.h"
#include "reg.h"

#define LOG_TAG "send"

// �ڴ������ڲ�����...
static int _WriteCmd(const FpgaRegBaseAddr* reg, const Fh228Priority priority, const int msgNo);
static int _WriteData(DevContext *dc, const uint32_t subAddr, const char *buf, const int bytes, ChnProp* chn);
static int _WriteDataBySmallPackage(
    ChnProp* chn,
    FpgaRegBaseAddr *reg,
    SendQueContext* sendQue,
    const char *buf,
    const int bytes
);

// ������д��DMA�ڵ�, ֪ͨFPGA
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

// ������д��DMA�ڵ�, ֪ͨFPGA, ��������
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

// �������г�С��, ��һ˳��д��DMA�ڵ���
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
    ChnProp* chn = CfgMgr_GetChnProp(&dc->cfgCtx, subAddr); // �����ӵ�ַ�ҵ���Ӧ�ķ���ͨ��
    if (!chn)
    {
        elog_e(LOG_TAG, "channel not found, sa=%u", subAddr);
        return -1;
    }

    if (CfgMgr_IsChnTx(chn)) // ��������
    {
        return _WriteData(dc, subAddr, buf, bytes, chn);
    }
    else // ��������
    {
        Statistics_AddNcCmdCount(chn->msgNo, chn->s_priority, 1);
        Statistics_AddNcCmdBytes(chn->msgNo, chn->s_priority, chn->s_dataByteCount);
        return _WriteCmd(dc->reg, chn->s_priority, chn->msgNo);
    }
}

bool Write_SetPreplanData(DevContext *dc, const uint32_t subAddr, const char *buf, const uint32_t bytes, const bool bForce)
{
    // �����ӵ�ַ�ҵ���Ӧ����Ϣͨ��, �ٵõ�ͨ������Ӧ��ƹ��buf
    ChnProp* chn = CfgMgr_GetChnProp(&dc->cfgCtx, subAddr);
    CsPingPongBufContext* pingpong = &chn->periodMsg;
    SendQueNode* node = NULL;
    if (!bForce) // ��ǿ�ƵĻ�, Ҫ�ж�����һ��Ԥ��������û�б�FPGA���͹�, ���û��, ����Ԥ��
    {
        node = (SendQueNode*)Cs_PingPongBufGetBuf(pingpong);
        if (false == SendQue_IsNodeWriteable(node))
        {
            elog_w(LOG_TAG, "sorry, old msg is not sent by fpga, so your need to wait");
            return false;
        }
    }

    Cs_PingPongBufSwap(pingpong); // �л�ƹ��BUG
    node = (SendQueNode*)Cs_PingPongBufGetBuf(pingpong);
    if (bytes > 0)
    {
        memcpy(node->dmaBuf->data, buf, bytes);
    }

    SendQue_SetNodeNonWriteable(node, bytes, bytes, 1, 1);
    Reg_PushChannelDmaBuf2Fpga(dc->reg, chn, sizeof(W0) + sizeof(W1) + bytes, node->index);

    // TODO: ʲôʱ�򼤻�������Ϣ, �д�ȷ��
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

    if (bytes != chn->s_dataByteCount) // ���ͷ���Ļ�, ���ݳ��ȱ���������еĳ���, ����һ��
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

    // ����ͨ����Ϣ, �ҵ���Ӧ�ķ��Ͷ���
    que = SendQueMgr_Get(&dc->sendQueMgr, chn->s_priority);
    if (!que)
    {
        elog_e(LOG_TAG, "sendQue not found, priority=%d", chn->s_priority);
        return -1;
    }

    // ���û�����, �г�С�� , �����Ͷ�������
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

    SendQue_Lock(sendQue); // ����
    // ��ȡ�������ͷ�ڵ�, �����ǲ��ǿ���������д����
    node = SendQue_PeekFront(sendQue);
    if (!SendQue_IsNodeWriteable(node))
    {
        goto Exit0;
    }

    if (payloadBytes > 0)
    {
        memcpy(node->dmaBuf->data, buf, payloadBytes); // ���û�Ҫ���͵�����, д���ڵ��DMA��
    }

    // ������ڵ���Ϊ����д. ��FPGAʹ��, FPGAʹ����, �����½������Ϊ��д
    SendQue_SetNodeNonWriteable(node, dataTotalBytes, payloadBytes, sop, eop);

    // ���ڵ����FPGA
    Reg_PushDmaBuf2Fpga(reg, node->index, sizeof(W0) + sizeof(W1) + payloadBytes, msgNo, eop, priority);
    SendQue_Move2NextNode(sendQue); // �ƶ�����һ���ڵ�
    Statistics_AddSendBytes(msgNo, priority, payloadBytes); // ͳ��

    suc = true;

Exit0:
    SendQue_Unlock(sendQue); // ����
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
 * @brief ���û�����, �г�С��, д��DMA�ڵ���, ����Ǻ�sop��eop
 * @return �ɹ����͵����ݴ�С
 * @attention ����ֵ�Ǳ����ͳ�ȥ�Ĵ�С, ������������ݴ�С, ����ʶ����ʧ��
 */
static int _WriteDataBySmallPackage(
    ChnProp* chn,
    FpgaRegBaseAddr *reg,
    SendQueContext* sendQue,
    const char *buf,
    const int bytes
)
{
    bool isFirst      = true; // �ǲ��ǵ�һ��
    bool suc          = false;
    int sentSum       = 0; // �ѷ��͵Ĵ�С
    int sop           = 0;
    int eop           = 0;
    int payloadBytes  = 0;
    int validDmaBytes = sendQueGetDmaBufSize(sendQue) - sizeof(W0) - sizeof(W1);
    char* payloadBuf  = NULL;
    CsFastBufSpliterContext spliter;

    Cs_FastBufSplitterAttach(&spliter, buf, bytes);

    do
    {
        payloadBytes = min(bytes - sentSum, validDmaBytes); // ��ʣ�����ݵĴ�С, ��DMA��Ч�Ĵ�С�Ƚ�, ȡ��С��
        payloadBuf = Cs_FastBufSplitterPop(&spliter, payloadBytes); // ��DMA���г�һ�ο�������paload��buf
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

    if (sentSum == bytes) // ���ͳɹ�, �Ӹ�ͳ��
    {
        Statistics_AddSendPackage(chn->msgNo, chn->s_priority, 1);
    }

    return sentSum;
}
