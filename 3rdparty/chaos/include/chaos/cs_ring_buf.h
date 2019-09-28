/**
 * @file
 * @brief �����λ�����
 *
 *    ������, �ײ�ʵ�ֻ��ڵ���������
 *    �������ڶ�ͷ����ɾ�Ĳ���
 *    �ڶ�β��ֻ���Ӳ���
 *    ��ͷ�ɵ������ƶ�����
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

#define CS_RING_BUF_NODE_OBJECT CsRingBufLink _csLink ///< ���Զ���Ľڵ�ṹ��, �����ڳ�Ա��������ǰ����������
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
 * @brief ������ͷ������һ�ڵ�
 * @param [in] context ������
 */
static void Cs_RingBufMove2NextNode(CsRingBufContext* context)
{
    assert(NULL != context);
    if (context->count > 1)   // 2����2�����ϵĽڵ�, ��ִ�л����ƶ�
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
