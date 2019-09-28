#include "read.h"
#include "chaos/cs_buf_writer.h"
#include "recv_que_mgr.h"
#include "elog4c/elog4c.h"
#include "banana/ba_inet.h"
#include "statistics.h"
#include "utils.h"

#define LOG_TAG "recvTask"
#define LOG_TAG_USER "userRead"

int Read_FromChnQue(DevContext *dc, const uint32_t subAddr, char *buf, const int length)
{
    const int MAX_WAIT_COUNT = 1000;
    int realLength           = 0;
    bool isSopFound          = false;
    int waitCnt              = 0;
    RecvQueNode* node        = NULL;
    bool suc                 = false;
    ChnProp* chn             = CfgMgr_GetChnProp(&dc->cfgCtx, subAddr);
    R0 r0;
    CsBufWriterContext ctx;

    if (!chn)
    {
        elog_e(LOG_TAG_USER, "channel not found, subAddr=%u", subAddr);
        goto Exit0;
    }

    Cs_BufWriterAttach(&ctx, buf, length);

    do
    {
        node = RecvQue_PopFront(&chn->recvQue);
        if (NULL == node)
        {
            // �Ƿ��Ѿ����˵�һ����, ���һ����û����, ֱ�ӷ���
            // ����Ѿ�����, ���ٶ��Լ���
            if (!isSopFound)
            {
                break;
            }

            waitCnt++;
            if (waitCnt > MAX_WAIT_COUNT)
            {
                elog_w(LOG_TAG_USER, "can not recv the eop of the package");
                goto Exit0;
            }

            Ba_TaskSleep(5);
            continue;
        }

        CastStruct2UInt32(r0) = Ba_NtohlStruct(node->dmaBuf->r0);
        if (1 == r0.sop)
        {
            // ��һ����ǰ4���ֽ�, ���ӵ�ַ, �����������, ����Ҫƫ��4���ֽ�
            isSopFound = true;
            Cs_BufWriterPushStr(&ctx, node->dmaBuf->data.ptr + 4, r0.length - 4);
        }
        else
        {
            // ֻ�����Ѿ���sop�������, �Ž������ಿ�ֵ����ݿ���
            if (isSopFound)
            {
                Cs_BufWriterPushStr(&ctx, node->dmaBuf->data.ptr, r0.length);
            }
            else
            {
                elog_w(LOG_TAG_USER, "package has no sop");
            }
        }
        RecvQueMgr_RecycleNode(&dc->recvQueMgr, node, chn->s_priority);

        if (1 == r0.eop)
        {
            break;
        }
    } while (1);

    suc = true;

Exit0:
    if (suc)
    {
        realLength = Cs_BufWriterGetOffset(&ctx);
    }
    else
    {
        realLength = -1;
        elog_e(LOG_TAG_USER, "user read data error");
    }

    return realLength;
}

bool Read_FromRecvQue(CfgMgrCtx* cfg, RecvQueMgrContext *recvQueMgr, Fh228Priority priority)
{
    ChnProp* chn = NULL;
    static int curSubAddr = -1; // ��¼��ǰ��������ݰ����ӵ�ַ
    R0 r0;
    RecvQueNode* node = NULL;
    RecvQueContext* recvQue = RecvQueMgr_Get(recvQueMgr, priority);
    if (!recvQue)
    {
        elog_e(LOG_TAG, "recv que not found, priority=%d", priority);
        return false;
    }

    // 1. �жϽڵ��Ƿ������ݿɶ�
    node = RecvQue_PeekFront(recvQue);
    if (!RecvQue_IsNodeReadable(node))
    {
        return false;
    }

    // 2. �жϽڵ��R0R1�Ƿ��������
    if (!RecvQue_IsNodeR0Ok(node))
    {
        RecvQue_PopFront(recvQue);
        RecvQueMgr_RecycleNode(recvQueMgr, node, priority);
        return false;
    }

    // 3. ����ǵ�һ����, ǰ4���ֽ����ӵ�ַ
    CastStruct2UInt32(r0) = Ba_NtohlStruct(node->dmaBuf->r0);
    if (1 == r0.sop)
    {
        curSubAddr = Ba_Ntohl(node->dmaBuf->data.subAddr);
    }

    // 4. �������ݰ��б�ʶ���ӵ�ַ, �ҵ���Ӧ��ͨ��
    chn = CfgMgr_GetChnProp(cfg, curSubAddr);
    if (!chn)
    {
        elog_e(LOG_TAG, "channel not found, curSubAddr=%d", curSubAddr);
        RecvQue_PopFront(recvQue);
        RecvQueMgr_RecycleNode(recvQueMgr, node, priority);
        return false;
    }

    // 5. ���ڵ�ӽ��ն������Ƴ�, �嵽��Ӧͨ���Ľ��ն��к���.
    RecvQue_PopFront(recvQue);
    RecvQue_PushBack(&chn->recvQue, node);

//     elog_d(LOG_TAG, "sa=%d, msgNo=%d, priority=%d, sop=%d, eop=%d, len=%u, node.idx=%d, node.la=%p",
//         curSubAddr, chn->msgNo, priority, r0.sop, r0.eop, r0.length, node->index, node->dmaBuf);

    // ͳ��
    if (1 == r0.sop)
    {
        // ��һ����ǰ4���ֽ�, ���ӵ�ַ, ��������
        Statistics_AddRecvBytes(chn->msgNo, priority, r0.length - 4);
    }
    else
    {
        Statistics_AddRecvBytes(chn->msgNo, priority, r0.length);
    }

    // �������һ��������, ���ӵ�ַ�����
    if (r0.eop == 1)
    {
        Statistics_AddRecvPackage(chn->msgNo, priority, 1);
        curSubAddr = -1;
    }

    return true;
}
