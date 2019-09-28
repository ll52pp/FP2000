/**
 * @file
 * @brief 单向环形缓冲区
 *
 *    适配器, 底层实现基于单向环形链表
 *    适用于在队头的增删改操作
 *    在队尾的只增加操作
 *    针头可单向环形移动操作
 * @author yaoyuliang(765495939@qq.com)
 **/

#ifndef CS_RING_BUF_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#define CS_RING_BUF_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#include "cs_list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef CsListLink CsRingBufLink;
typedef CsListContext CsRingBufContext;

#define CS_RING_BUF_NODE_OBJECT CsRingBufLink _csLink ///< 你自定义的节点结构体, 必须在成员变量的最前面加上这个宏
#define _CS_CAST2LIST_CONTEXT(x) ((CsListContext*)(x))

#define Cs_RingBufInit(context)             Cs_ListInit(_CS_CAST2LIST_CONTEXT(context))
#define Cs_RingBufUninit(context)           Cs_ListUninit(_CS_CAST2LIST_CONTEXT(context))
#define Cs_RingBufNodeMalloc(size)          Cs_ListNodeMalloc(size)
#define Cs_RingBufNodeFree(node)            Cs_ListNodeFree(node)
#define Cs_RingBufPushBack(context, node)   Cs_ListPushBack(_CS_CAST2LIST_CONTEXT(context), (node))
#define Cs_RingBufPushFront(context, node)  Cs_ListPushFront(_CS_CAST2LIST_CONTEXT(context), (node))
#define Cs_RingBufGetSize(context)          Cs_ListGetSize(_CS_CAST2LIST_CONTEXT(context))
#define Cs_RingBufPeekFront(context)        Cs_ListPeekFront(_CS_CAST2LIST_CONTEXT(context))
#define Cs_RingBufPopFront(context)         Cs_ListPopFront(_CS_CAST2LIST_CONTEXT(context))
#define Cs_RingBufNext(node)                Cs_ListNext((node))

/**
 * @brief 将操作头移向到下一节点
 * @param [in] context 上下文
 */
static void Cs_RingBufMove2NextNode(CsRingBufContext* context)
{
    assert(NULL != context);
    if (context->count > 1)   // 2个或2个以上的节点, 才执行环形移动
    {
        CsRingBufLink* first = Cs_ListPeekFront(_CS_CAST2LIST_CONTEXT(context));
        context->head = first->next;
        context->tail->next = first;
        context->tail = first;
        first->next = NULL;
    }
}

#ifdef __cplusplus
}
#endif

#endif // CS_RING_BUF_H_3935E820_DD02_4DD7_AC55_9F92796BC977
