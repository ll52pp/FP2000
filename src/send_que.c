#include "send_que.h"
#include "utils.h"

// 内部函数在此声明...
static SendQueNode* SendQue_NodeMalloc(SendQueContext* context);
static void SendQue_NodeFree(SendQueNode* node);
static void SendQue_InitRingNodes(SendQueContext* context, int msgNumBase);
static void SendQue_UninitRingNodes(SendQueContext* context);

void SendQue_Init(SendQueContext* context, int dmaBufCnt, int dmaBufSize, int msgNumBase)
{
    // 初始化共2步:
    // 1. 新建一个指定大小的链表
    // 2. 为链表的每个节点, 申请一块DMA buf

    context->dmaCnt = dmaBufCnt;
    context->dmaSize = dmaBufSize;
    Cs_RingBufInit(&context->ringList);
    SendQue_InitRingNodes(context, msgNumBase);
    Ba_MutexInit(&context->lock);
}

void SendQue_Uninit(SendQueContext* context)
{
    // 反初始化共2步:
    // 1. 释放链表上所有节点所持有的DMA buf
    // 2. 释放整个链表

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
    for (; i < 100000; i++) // 这个地方用i防止进入死循环
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
    const int alignBytes = 1024; // 指定多少字节对齐
    SendQueNode* node = (SendQueNode*)Cs_RingBufNodeMalloc(sizeof(SendQueNode));

    // 因为FPGA要求DMA的地址, 必须1K对齐, 所以这个地方, 需要对申请到的DMA地址做下1024对齐
    // 会造成一定的内存浪费, 不过无所谓啦, 释放的时候, 注意别释放错了, 释放成对齐后的地址了
    node->rawDmaBuf = (char*)Ba_DmaAlloc(context->dmaSize + alignBytes);
    node->dmaBuf = (SendQueDmaNode*)Cs_CalcByteAlignment((uint32_t)node->rawDmaBuf, alignBytes);

    // 赋个初始值
    memset(node->dmaBuf, 0, context->dmaSize);
    node->dmaBuf->w0.flag = 1;
    CastStruct2UInt32(node->dmaBuf->w0) = Ba_HtonlStruct(node->dmaBuf->w0);
    return node;
}
