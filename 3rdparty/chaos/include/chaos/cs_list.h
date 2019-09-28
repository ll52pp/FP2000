/**
 * @file
 * @brief ��������
 *
 * ������ͷ����������,ɾ������, β��ֻ֧�����Ӳ���, �м�������
 * @author yaoyuliang(765495939@qq.com)
 */

#ifndef CS_LIST_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#define CS_LIST_H_3935E820_DD02_4DD7_AC55_9F92796BC977

#include "cs_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CsListLinkTag
{
    struct CsListLinkTag *next; ///< ָ����һ�ڵ�
} CsListLink;

typedef struct
{
    struct CsListLinkTag*   head;  ///< ͷ
    struct CsListLinkTag*   tail;  ///< β
    int                     count; ///< �ڵ����
} CsListContext;

#define CS_LIST_NODE_OBJECT CsListLink _csLink ///< ���Զ���Ľڵ�ṹ��, �����ڳ�Ա��������ǰ����������
#define _CS_CAST2LIST_NODE(x) ((CsListLink*)(x))

static void  Cs_ListInit(CsListContext *context);
static void  Cs_ListUninit(CsListContext *context);
static void* Cs_ListNodeMalloc(unsigned int nodeSize);
static void  Cs_ListInsert(CsListContext *context, void *prev, void *node);
static void  Cs_ListPushBack(CsListContext *context, void *node);
static void  Cs_ListPushFront(CsListContext *context, void *node);
static void* Cs_ListPopFront(CsListContext *context);
static int   Cs_ListGetSize(CsListContext *context);
static void* Cs_ListNext(void *node);
static void* Cs_ListPeekFront(CsListContext *context);

/**
 * @fn Cs_ListNodeFree
 * @brief �ͷŽڵ�
 * @param [in] context ������
 * @param [in] node �ڵ��׵�ַ
 */
#define Cs_ListNodeFree(node) Cs_Free(node)


//////////////////////////////////////////////////////////////////////////
// �����Ǻ���ʵ�ֲ���...

/**
 * @brief ��ʼ��
 * @param [in] context ������
 */
static void Cs_ListInit(CsListContext *context)
{
    context->head = NULL;
    context->tail = NULL;
    context->count = 0;
}
/**
 * @brief ����ʼ������, �ͷ����������ϵĽڵ�
 * @param [in] context ������
 */
static void Cs_ListUninit(CsListContext *context)
{
    CsListLink *p1, *p2;
    if (context->count > 0)
    {
        p1 = context->head;
        while (p1 != NULL)
        {
            p2 = p1->next;
            Cs_Free(p1);
            p1 = p2;
        }
        context->count = 0;
        context->head = context->tail = NULL;
    }
}

/**
 * @brief ����һ���½ڵ�
 * @param [in] nodeSize �ڵ��С
 * @return �ڵ��׵�ַ
 */
static void* Cs_ListNodeMalloc(unsigned int nodeSize)
{
    CsListLink* link = (CsListLink*)Cs_Malloc(nodeSize);
    link->next = NULL;
    return link;
}

/**
 * @brief �ں󷽲���ڵ�
 * @param [in] context ����context
 * @param [in] prev ��Ҫ�������λ��, ��������ڵ�ĺ󷽲���
 * @param [in] node �����������Ľڵ�
 */
static void Cs_ListInsert(CsListContext *context, void *prev, void *node)
{
    CsListLink *next;

    if (prev == NULL)
    {
        next = context->head;
        context->head = node;
    }
    else
    {
        next = _CS_CAST2LIST_NODE(prev)->next;
        _CS_CAST2LIST_NODE(prev)->next = node;
    }

    if (next == NULL)
    {
        context->tail = node;
    }

    _CS_CAST2LIST_NODE(node)->next = next;
    context->count++;
}

/**
 * @brief ���ڵ�׷�ӵ������β��
 * @param [in] context ������
 * @param [in] node �ڵ��׵�ַ
 */
static void Cs_ListPushBack(CsListContext *context, void *node)
{
    Cs_ListInsert(context, context->tail, node);
}

/**
 * @brief ���ڵ���������ͷ��
 * @param [in] context ������
 * @param [in] node �ڵ��׵�ַ
 */
static void Cs_ListPushFront(CsListContext *context, void *node)
{
    CsListLink* firstNode = context->head;
    if (firstNode)
    {
        _CS_CAST2LIST_NODE(node)->next = firstNode;
        context->head = node;
        context->count++;
    }
    else
    {
        Cs_ListInsert(context, context->tail, node);
    }
}

/**
 * @brief ���������ͷ�ڵ�, ���Ƴ���һ���ڵ�
 * @param [in] context ������
 * @return ������ͷ�ڵ�ĵ�ַ
 */
static void* Cs_ListPopFront(CsListContext *context)
{
    CsListLink *head = context->head;

    if (head != NULL)
    {
        context->head = head->next;
        if (head->next == NULL)
        {
            context->tail = NULL;
        }
        context->count--;
    }

    return head;
}

/**
 * @brief ��ȡ����ڵ�ĸ���
 * @param [in] context ������
 * @return �ڵ�ĸ���
 */
static int Cs_ListGetSize(CsListContext *context)
{
    return (context->count);
}

/**
 * @brief ��ȡָ���ڵ��Ӧ����һ���ڵ��ַ
 * @param [in] node ָ���ڵ���׵�ַ
 * @return ��һ���ڵ���׵�ַ
 */
static void* Cs_ListNext(void *node)
{
    return (_CS_CAST2LIST_NODE(node)->next);
}

/**
 * @brief ��ȡ�����ͷ�ڵ�
 * @param [in] context ������
 * @attention ע��, peek��pop����������, pop�Ὣ�ڵ���������Ƴ�, ��peek����
 * @return ͷ�ڵ��׵�ַ
 */
static void* Cs_ListPeekFront(CsListContext *context)
{
    return (context->head);
}

#ifdef __cplusplus
}
#endif

#endif
