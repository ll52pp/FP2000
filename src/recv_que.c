#include "recv_que.h"
#include "utils.h"


// �ڲ������ڴ�����...
static RecvQueNode* RecvQue_NodeMalloc(RecvQueContext* context);
static void RecvQue_NodeFree(RecvQueNode* node);
static void RecvQue_InitRingNodes(RecvQueContext* context, int msgNumBase);
static void RecvQue_UninitRingNodes(RecvQueContext* context);

void RecvQue_Init(RecvQueContext* context, int dmaBufCnt, int dmaBufSize, int startOfIndex)
{
    context->dmaCnt = dmaBufCnt;
    context->dmaSize = dmaBufSize;
    Ba_MutexInit(&context->_mutex); // ��ʼ����
    Cs_RingBufInit(&context->ringList); // ��ʼ����������
    RecvQue_InitRingNodes(context, startOfIndex); // ��ʼ����������ĸ����ڵ�
}

void RecvQue_Uninit(RecvQueContext* context)
{
    RecvQue_UninitRingNodes(context);
    Cs_RingBufUninit(&context->ringList);
    Ba_MutexUninit(&context->_mutex);
    context->dmaCnt = 0;
    context->dmaSize = 0;
}

int RecvQue_GetDmaBufSize(RecvQueContext* context)
{
    return context->dmaSize;
}

void RecvQue_PushBack(RecvQueContext* context, RecvQueNode* node)
{
    Ba_MutexLock(&context->_mutex);
    Cs_RingBufPushBack(&context->ringList, node);
    Ba_MutexUnlock(&context->_mutex);
}

RecvQueNode* RecvQue_PeekFront(RecvQueContext* context)
{
    RecvQueNode* node = NULL;
    Ba_MutexLock(&context->_mutex);
    node = (RecvQueNode*)Cs_RingBufPeekFront(&context->ringList);
    Ba_MutexUnlock(&context->_mutex);
    return node;
}

RecvQueNode* RecvQue_PopFront(RecvQueContext* context)
{
    RecvQueNode* node = NULL;
    Ba_MutexLock(&context->_mutex);
    node = (RecvQueNode*)Cs_RingBufPopFront(&context->ringList);
    Ba_MutexUnlock(&context->_mutex);
    return node;
}

void RecvQue_Move2NextNode(RecvQueContext* context)
{
    Ba_MutexLock(&context->_mutex);
    Cs_RingBufMove2NextNode(&context->ringList);
    Ba_MutexUnlock(&context->_mutex);
}

bool RecvQue_IsNodeReadable(RecvQueNode* node)
{
    R0 r0;
    if (node)
    {
        CastStruct2UInt32(r0) = Ba_NtohlStruct(node->dmaBuf->r0);
        if (0 != r0.length)
        {
            return true;
        }
    }

    return false;
}

void RecvQue_SetNodeEmpty(RecvQueNode* node)
{
    ZeroStruct(node->dmaBuf->r0);
    ZeroStruct(node->dmaBuf->r1);
    node->dmaBuf->data.subAddr = 0xffffffff;
}

void RecvQue_InitRingNodes(RecvQueContext* context, int msgNumBase)
{
    int i = 0;
    for (; i < context->dmaCnt; i++)
    {
        RecvQueNode* node = RecvQue_NodeMalloc(context);
        node->index = msgNumBase;
        Cs_RingBufPushBack(&context->ringList, node);
        msgNumBase++;
    }
}

void RecvQue_UninitRingNodes(RecvQueContext* context)
{
    int i = 0;
    for (; i < 100000; i++) // ����ط���i��ֹ������ѭ��
    {
        RecvQueNode* front = (RecvQueNode*)Cs_RingBufPopFront(&context->ringList);
        if (front)
        {
            RecvQue_NodeFree(front);
        }
        else
        {
            break;
        }
    }
    assert(i < 100000);
}

RecvQueNode* RecvQue_NodeMalloc(RecvQueContext* context)
{
    const int alignBytes = 1024; // ָ�������ֽڶ���
    RecvQueNode* node = (RecvQueNode*)Cs_RingBufNodeMalloc(sizeof(RecvQueNode));

    // ��ΪFPGAҪ��DMA�ĵ�ַ, ����1K����, ��������ط�, ��Ҫ�����뵽��DMA��ַ����1024����
    // �����һ�����ڴ��˷�, ��������ν��, �ͷŵ�ʱ��, ע����ͷŴ���, �ͷųɶ����ĵ�ַ��
    node->rawDmaBuf = (char*)Ba_DmaAlloc(context->dmaSize + alignBytes);
    node->dmaBuf = (RecvQueDmaNode*)Cs_CalcByteAlignment((uint32_t)node->rawDmaBuf, alignBytes);
    memset(node->dmaBuf, 0, context->dmaSize);
    return node;
}

void RecvQue_NodeFree(RecvQueNode* node)
{
    Ba_DmaFree(node->rawDmaBuf);
    Cs_RingBufNodeFree(node);
}

bool RecvQue_IsNodeR0Ok(RecvQueNode* node)
{
    R0 r0;
    UINT32 subAddr = 0;

    CastStruct2UInt32(r0) = Ba_NtohlStruct(node->dmaBuf->r0);
    subAddr = Ba_Ntohl(node->dmaBuf->data.subAddr);

    if ((r0.frame_err == 1) ||
        (r0.ox_id_err == 1) ||
        (r0.seq_err == 1))
    {
        elog_e("recvTask", "error in r0, r0.frame_err=%d, r0.ox_id_err=%d, r0.seq_err=%d",
            r0.frame_err, r0.ox_id_err, r0.seq_err);
        return false;
    }

    // ����ǵ�һ����, �ж�һ����û�б�ʶ�ӵ�ַ, ���Ϊ0, ����Ч
    if (r0.sop == 1 && subAddr == 0)
    {
        elog_e("recvTask", "error in r0, sop has no sub address");
        return false;
    }

    // ��Ϊ��һ����ǰ4���ֽ�, ������ӵ�ַ, ���Բ�������4�ֽ�
    if (r0.sop == 1 && r0.length < 4)
    {
        elog_e("recvTask", "error in r0, sop.len is less than 4bytes, r0.len=%u", r0.length);
        return false;
    }

    return true;
}
