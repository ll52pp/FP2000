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
            // 是否已经收了第一包了, 如果一包都没收呢, 直接返回
            // 如果已经收了, 则再多试几次
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
            // 第一包的前4个字节, 是子地址, 后面才是数据, 所以要偏移4个字节
            isSopFound = true;
            Cs_BufWriterPushStr(&ctx, node->dmaBuf->data.ptr + 4, r0.length - 4);
        }
        else
        {
            // 只有在已经有sop的情况下, 才进行其余部分的数据拷贝
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
    static int curSubAddr = -1; // 记录当前处理的数据包的子地址
    R0 r0;
    RecvQueNode* node = NULL;
    RecvQueContext* recvQue = RecvQueMgr_Get(recvQueMgr, priority);
    if (!recvQue)
    {
        elog_e(LOG_TAG, "recv que not found, priority=%d", priority);
        return false;
    }

    // 1. 判断节点是否有数据可读
    node = RecvQue_PeekFront(recvQue);
    if (!RecvQue_IsNodeReadable(node))
    {
        return false;
    }

    // 2. 判断节点的R0R1是否是正解的
    if (!RecvQue_IsNodeR0Ok(node))
    {
        RecvQue_PopFront(recvQue);
        RecvQueMgr_RecycleNode(recvQueMgr, node, priority);
        return false;
    }

    // 3. 如果是第一个包, 前4个字节是子地址
    CastStruct2UInt32(r0) = Ba_NtohlStruct(node->dmaBuf->r0);
    if (1 == r0.sop)
    {
        curSubAddr = Ba_Ntohl(node->dmaBuf->data.subAddr);
    }

    // 4. 根据数据包中标识的子地址, 找到对应的通道
    chn = CfgMgr_GetChnProp(cfg, curSubAddr);
    if (!chn)
    {
        elog_e(LOG_TAG, "channel not found, curSubAddr=%d", curSubAddr);
        RecvQue_PopFront(recvQue);
        RecvQueMgr_RecycleNode(recvQueMgr, node, priority);
        return false;
    }

    // 5. 将节点从接收队列中移出, 插到对应通道的接收队列后面.
    RecvQue_PopFront(recvQue);
    RecvQue_PushBack(&chn->recvQue, node);

//     elog_d(LOG_TAG, "sa=%d, msgNo=%d, priority=%d, sop=%d, eop=%d, len=%u, node.idx=%d, node.la=%p",
//         curSubAddr, chn->msgNo, priority, r0.sop, r0.eop, r0.length, node->index, node->dmaBuf);

    // 统计
    if (1 == r0.sop)
    {
        // 第一包的前4个字节, 是子地址, 不是数据
        Statistics_AddRecvBytes(chn->msgNo, priority, r0.length - 4);
    }
    else
    {
        Statistics_AddRecvBytes(chn->msgNo, priority, r0.length);
    }

    // 若是最后一包数据了, 则将子地址清理掉
    if (r0.eop == 1)
    {
        Statistics_AddRecvPackage(chn->msgNo, priority, 1);
        curSubAddr = -1;
    }

    return true;
}
