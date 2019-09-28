#include "send_que.h"
#include "utils.h"

// �ڲ������ڴ�����...
static SendQueNode* SendQue_NodeMalloc(SendQueContext* context);
static void SendQue_NodeFree(SendQueNode* node);
static void SendQue_InitRingNodes(SendQueContext* context, int msgNumBase);
static void SendQue_UninitRingNodes(SendQueContext* context);

void SendQue_Init(SendQueContext* context, int dmaBufCnt, int dmaBufSize, int msgNumBase)
{
    // ��ʼ����2��:
    // 1. �½�һ��ָ����С������
    // 2. Ϊ�����ÿ���ڵ�, ����һ��DMA buf

    context->dmaCnt = dmaBufCnt;
    context->dmaSize = dmaBufSize;
    Cs_RingBufInit(&context->ringList);
    SendQue_InitRingNodes(context, msgNumBase);
    Ba_MutexInit(&context->lock);
}

void SendQue_Uninit(SendQueContext* context)
{
    // ����ʼ����2��:
    // 1. �ͷ����������нڵ������е�DMA buf
    // 2. �ͷ���������

    Ba_MutexUninit(&context->lock);
    SendQue_UninitRingNodes(context);
    Cs_RingBufUninit(&context->ringList);
}

void SendQue_Lock(SendQueContext* context)
{
    Ba_MutexLock(&context->lock);
}

void SendQue_Unlock(SendQueContext* context)
{
    Ba_MutexUnlock(&context->lock);
}

int SendQue_GetNodeCount(SendQueContext* context)
{
    return context->dmaCnt;
}

int SendQue_GetNodeSize(SendQueContext* context)
{
    return context->dmaSize;
}

void SendQue_PushBack(SendQueContext* context, SendQueNode* node)
{
    Cs_RingBufPushBack(&context->ringList, node);
}

SendQueNode* SendQue_PeekFront(SendQueContext* context)
{
    SendQueNode* node = NULL;
    node = (SendQueNode*)Cs_RingBufPeekFront(&context->ringList);
    return node;
}

void SendQue_Move2NextNode(SendQueContext* context)
{
    Cs_RingBufMove2NextNode(&context->ringList);
}

bool SendQue_IsNodeWriteable(SendQueNode* node)
{
    W0 w0;
    CastStruct2UInt32(w0) = Ba_NtohlStruct(node->dmaBuf->w0);
    if (w0.flag == 0)
    {
        return false;
    }

    return true;
}

void SendQue_SetNodeNonWriteable(SendQueNode* node, const int iuLength, const int payloadLen, const int sop, const int eop)
{
    W0 w0;
    W1 w1;

    ZeroStruct(w0);
    ZeroStruct(w1);

    w0.flag = 0;
    w0.length = sizeof(W0) + sizeof(W1) + payloadLen;
    w1.length = iuLength;
    w0.sop = sop;
    w0.eop = eop;

    CastStruct2UInt32(node->dmaBuf->w0) = Ba_HtonlStruct(w0);
    CastStruct2UInt32(node->dmaBuf->w1) = Ba_HtonlStruct(w1);
}

void SendQue_InitRingNodes(SendQueContext* context, int msgNumBase)
{
    int i = 0;
    for (; i < context->dmaCnt; i++)
    {
        SendQueNode* node = SendQue_NodeMalloc(context);
        node->index = msgNumBase;
        Cs_RingBufPushBack(&context->ringList, node);
        msgNumBase++;
    }
}

void SendQue_UninitRingNodes(SendQueContext* context)
{
    int i = 0;
    for (; i < 100000; i++) // ����ط���i��ֹ������ѭ��
    {
        SendQueNode* front = (SendQueNode*)Cs_RingBufPopFront(&context->ringList);
        if (front)
        {
            SendQue_NodeFree(front);
        }
        else
        {
            break;
        }
    }
    assert(i < 100000);
}

void SendQue_NodeFree(SendQueNode* node)
{
    Ba_DmaFree(node->rawDmaBuf);
    Cs_RingBufNodeFree(node);
}

SendQueNode* SendQue_NodeMalloc(SendQueContext* context)
{
    const int alignBytes = 1024; // ָ�������ֽڶ���
    SendQueNode* node = (SendQueNode*)Cs_RingBufNodeMalloc(sizeof(SendQueNode));

    // ��ΪFPGAҪ��DMA�ĵ�ַ, ����1K����, ��������ط�, ��Ҫ�����뵽��DMA��ַ����1024����
    // �����һ�����ڴ��˷�, ��������ν��, �ͷŵ�ʱ��, ע����ͷŴ���, �ͷųɶ����ĵ�ַ��
    node->rawDmaBuf = (char*)Ba_DmaAlloc(context->dmaSize + alignBytes);
    node->dmaBuf = (SendQueDmaNode*)Cs_CalcByteAlignment((uint32_t)node->rawDmaBuf, alignBytes);

    // ������ʼֵ
    memset(node->dmaBuf, 0, context->dmaSize);
    node->dmaBuf->w0.flag = 1;
    CastStruct2UInt32(node->dmaBuf->w0) = Ba_HtonlStruct(node->dmaBuf->w0);
    return node;
}
