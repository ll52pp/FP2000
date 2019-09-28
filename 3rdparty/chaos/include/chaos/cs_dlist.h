/**
 * @file
 * @brief ˫��������
 *
 * ��������β������ɾ�Ĳ���, �м�������
 * @author yaoyuliang(765495939@qq.com)
 *
 * �������ݽṹ:
 *
 *```
 *   1. ������
 *         -----------
 *         |  head------------------
 *         |         |             |
 *         |  tail----------       |
 *         |         |     |       v
 *         | count=0 |   -----   -----
 *         -----------    ---     ---
 *                         -       -
 *
 *   2. �ǿ�����
 *    ---------             --------          --------
 *    | head--------------->| next----------->| next---------
 *    |       |             |      |          |      |      |
 *    |       |       ------- prev |<---------- prev |      |
 *    |       |       |     |      |          |      |      |
 *    | tail------    |     | ...  |    ----->| ...  |      |
 *    |       |  |    v                 |                   v
 *    |count=2|  |  -----               |                 -----
 *    ---------  |   ---                |                  ---
 *               |    -                 |                   -
 *               |                      |
 *               ------------------------
 *```
 */

#ifndef CS_DLIST_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#define CS_DLIST_H_3935E820_DD02_4DD7_AC55_9F92796BC977

#include "cs_base.h"

#define _CS_DLIST_HEAD    link.next
#define _CS_DLIST_TAIL    link.prev
#define _CS_CAST2DLIST_LINK(x) ((CsDListLink*)(x))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CsDListLinkTag
{
    struct CsDListLinkTag *next;
    struct CsDListLinkTag *prev;
} CsDListLink;

typedef struct
{
    CsDListLink link; ///< ָ����һ�ڵ�
    int         count; ///< �ڵ����
} CsDListContext;

#define CS_DLIST_NODE_OBJECT CsDListLink _csDLink ///< ���Զ���Ľڵ�ṹ��, �����ڳ�Ա��������ǰ����������

static void  Cs_DListInit(CsDListContext *context);
static void  Cs_DListUninit(CsDListContext *context);
static void* Cs_DListNodeMalloc(unsigned int nodeSize);
static void  Cs_DListInsert(CsDListContext *context, void *prev, void *node);
static void  Cs_DListPushFront(CsDListContext *context, void *node);
static void  Cs_DListPushBack(CsDListContext *context, void *node);
static void* Cs_DListPopFront(CsDListContext *context);
static void* Cs_DListPopBack(CsDListContext *context);
static void  Cs_DListDelete(CsDListContext *context, void *node);
static void  Cs_DListConcat(CsDListContext *dstContext, CsDListContext *srcContext);
static void  Cs_DListExtract(CsDListContext *srcContext, void *startNode, void *endNode, CsDListContext *dstContext);
static int   Cs_DListFind(CsDListContext *context, void *node);
static void* Cs_DListPeekAt(CsDListContext *context, int index);
static void* Cs_DListNStep(void *node, int step);

#define Cs_DListNodeFree(node) Cs_Free(node)
#define Cs_DListGetSize(context) ((context)->count)
#define Cs_DListNext(node) (_CS_CAST2DLIST_LINK(node)->next)
#define Cs_DListPrevious(node) (_CS_CAST2DLIST_LINK(node)->prev)
#define Cs_DListPeekFront(context) ((void*)((context)->_CS_DLIST_HEAD))
#define Cs_DListPeekBack(context) ((void*)((context)->_CS_DLIST_TAIL))

//////////////////////////////////////////////////////////////////////////
// �����Ǻ���ʵ�ֲ���...

/**
 * @brief ��ʼ��
 * @param [in] context ������
 */
static void Cs_DListInit(CsDListContext *context)
{
    assert(NULL != context);
    context->_CS_DLIST_HEAD = NULL;
    context->_CS_DLIST_TAIL = NULL;
    context->count = 0;
}

/**
 * @brief ����ʼ��
 * @param [in] context ������
 */
static void Cs_DListUninit(CsDListContext *context)
{
    CsDListLink *p1 = NULL;
    CsDListLink *p2 = NULL;

    if (context->count > 0)
    {
        p1 = context->_CS_DLIST_HEAD;
        while (p1 != NULL)
        {
            p2 = p1->next;
            Cs_Free(p1);
            p1 = p2;
        }
        context->count = 0;
        context->_CS_DLIST_HEAD = context->_CS_DLIST_TAIL = NULL;
    }
}

/**
 * @brief ����һ��ָ����С������ڵ�
 * @param [in] nodeSize �ڵ��С
 * @return �������뵽�Ľڵ�ĵ�ַ
 */
static void* Cs_DListNodeMalloc(unsigned int nodeSize)
{
    CsDListLink* link = (CsDListLink*)Cs_Malloc(nodeSize);
    assert(NULL != link);
    link->next = NULL;
    link->prev = NULL;
    return link;
}

/**
 * @brief �������ָ���ڵ�λ�ú�, ����һ���½ڵ�
 * @param [in] context ������
 * @param [in] prev ָ��Ҫ������Ľڵ�
 * @param [in] node �µĴ�������Ľڵ�
 */
static void Cs_DListInsert(CsDListContext *context, void *prev, void *node)
{
    CsDListLink *next;

    if (prev == NULL)
    {
        next = context->_CS_DLIST_HEAD;
        context->_CS_DLIST_HEAD = node;
    }
    else
    {
        next = _CS_CAST2DLIST_LINK(prev)->next;
        _CS_CAST2DLIST_LINK(prev)->next = node;
    }

    if (next == NULL)
    {
        context->_CS_DLIST_TAIL = node;
    }
    else
    {
        next->prev = node;
    }

    _CS_CAST2DLIST_LINK(node)->next = next;
    _CS_CAST2DLIST_LINK(node)->prev = prev;

    context->count++;
}

/**
 * @brief ������ͷ�������½ڵ�
 * @param [in] context ������
 * @param [in] node ��������½ڵ�
 */
static void Cs_DListPushFront(CsDListContext *context, void *node)
{
    CsDListLink* firstNode = context->_CS_DLIST_HEAD;
    if (firstNode)
    {
        _CS_CAST2DLIST_LINK(node)->prev = firstNode->prev;
        _CS_CAST2DLIST_LINK(node)->next = firstNode;
        firstNode->prev = node;
        context->_CS_DLIST_HEAD = node;
        context->count++;
    }
    else
    {
        Cs_DListInsert(context, context->_CS_DLIST_TAIL, node);
    }
}

/**
 * @brief ������β�������½ڵ�
 * @param [in] context ������
 * @param [in] node ��������½ڵ�
 */
static void Cs_DListPushBack(CsDListContext *context, void *node)
{
    assert(NULL != context);
    assert(NULL != node);
    Cs_DListInsert(context, context->_CS_DLIST_TAIL, node);
}

/**
 * @brief ����ͷ�ڵ�
 * @param [in] context ������
 * @return ���ؽڵ��ַ
 */
static void* Cs_DListPopFront(CsDListContext *context)
{
    CsDListLink *head = context->_CS_DLIST_HEAD;

    if (head != NULL)
    {
        context->_CS_DLIST_HEAD = head->next;
        if (head->next == NULL)
        {
            context->_CS_DLIST_TAIL = NULL;
        }
        else
        {
            head->next->prev = NULL;
        }
        context->count--;
    }

    return head;
}

/**
 * @brief ����β�ڵ�
 * @param [in] context ������
 * @return ���ؽڵ��ַ
 */
static void* Cs_DListPopBack(CsDListContext *context)
{
    CsDListLink* tail = context->_CS_DLIST_TAIL;
    if (tail != NULL)
    {
        context->_CS_DLIST_TAIL = tail->prev;
        if (tail->prev == NULL)
        {
            context->_CS_DLIST_HEAD = NULL;
        }
        else
        {
            tail->prev->next = NULL;
        }
        context->count--;
    }

    return tail;
}

/**
 * @brief ���������Ƴ�ָ���Ľڵ�, �����ͷŽڵ�
 * @param [in] context ������
 * @param [in] node Ҫ�Ƴ��Ľڵ�
 * @attention ֻ�Ǵ��������Ƴ�, �����ͷŽڵ�ռ�õ��ڴ�, ע����ڴ�й©
 */
static void Cs_DListDelete(CsDListContext *context, void *node)
{
    if (_CS_CAST2DLIST_LINK(node)->prev == NULL)
    {
        context->_CS_DLIST_HEAD = _CS_CAST2DLIST_LINK(node)->next;
    }
    else
    {
        _CS_CAST2DLIST_LINK(node)->prev->next = _CS_CAST2DLIST_LINK(node)->next;
    }

    if (_CS_CAST2DLIST_LINK(node)->next == NULL)
    {
        context->_CS_DLIST_TAIL = _CS_CAST2DLIST_LINK(node)->prev;
    }
    else
    {
        _CS_CAST2DLIST_LINK(node)->next->prev = _CS_CAST2DLIST_LINK(node)->prev;
    }

    context->count--;
}

/**
 * @brief ��2������, �ϲ���һ������
 * @param [out] dstContext Ŀ������
 * @param [in] srcContext Դ����
 * @attention ��Ϊ���漰�ڵ�Ŀ���, ����ֱ�ӽ�Դ��������нڵ�, ָ��Ŀ������β��, ����Դ���������, ����ɿ�����
 */
static void Cs_DListConcat(CsDListContext *dstContext, CsDListContext *srcContext)
{
    assert(NULL != dstContext);
    assert(NULL != srcContext);

    if (srcContext->count == 0)
    {
        return;
    }

    if (dstContext->count == 0)
    {
        *dstContext = *srcContext;
    }
    else
    {
        dstContext->_CS_DLIST_TAIL->next = srcContext->_CS_DLIST_HEAD;
        srcContext->_CS_DLIST_HEAD->prev = dstContext->_CS_DLIST_TAIL;
        dstContext->_CS_DLIST_TAIL = srcContext->_CS_DLIST_TAIL;
        dstContext->count += srcContext->count;
    }

    Cs_DListInit(srcContext);
}

/**
 * @brief ��Դ�����е�һ��, ����ʼ��Ŀ������
 * @param [in] srcContext Դ����������
 * @param [in] startNode Դ������Ҫ����ȡ����ʼ�ڵ�
 * @param [in] endNode Դ������Ҫ����ȡ�Ľ����ڵ�
 * @param [out] dstContext Ŀ������, ����Ϊ������, ����û��ʼ����
 * @attention ����Դ����ָ����һ��, ����ʼ��Ŀ�������, ����Ŀ������, ����Ϊδ��ʼ��
 */
static void Cs_DListExtract(CsDListContext *srcContext, void *startNode, void *endNode, CsDListContext *dstContext)
{
    int i;
    CsDListLink *node;

    if (_CS_CAST2DLIST_LINK(startNode)->prev == NULL)
    {
        srcContext->_CS_DLIST_HEAD = _CS_CAST2DLIST_LINK(endNode)->next;
    }
    else
    {
        _CS_CAST2DLIST_LINK(startNode)->prev->next = _CS_CAST2DLIST_LINK(endNode)->next;
    }

    if (_CS_CAST2DLIST_LINK(endNode)->next == NULL)
    {
        srcContext->_CS_DLIST_TAIL = _CS_CAST2DLIST_LINK(startNode)->prev;
    }
    else
    {
        _CS_CAST2DLIST_LINK(endNode)->next->prev = _CS_CAST2DLIST_LINK(startNode)->prev;
    }

    dstContext->_CS_DLIST_HEAD = startNode;
    dstContext->_CS_DLIST_TAIL = endNode;

    _CS_CAST2DLIST_LINK(startNode)->prev = NULL;
    _CS_CAST2DLIST_LINK(endNode)->next = NULL;

    i = 0;

    for (node = startNode; node != NULL; node = node->next)
    {
        i++;
    }

    srcContext->count -= i;
    dstContext->count = i;
}

/**
 * @brief ����ָ���ڵ����ڵ�λ���±�
 * @param [in] context ������
 * @param [in] node Ҫ���ҵĽڵ�
 * @return �ڵ��������е�λ���±�
 */
static int Cs_DListFind(CsDListContext *context, void *node)
{
    CsDListLink *next;
    int index = 1;

    next = Cs_DListPeekFront(context);

    while ((next != NULL) && (next != node))
    {
        index++;
        next = Cs_DListNext(next);
    }

    if (next == NULL)
    {
        return -1;
    }
    else
    {
        return (index - 1);
    }
}

/**
 * @brief ȡ��ָ��λ���±�Ľڵ�
 * @param [in] context ������
 * @param [in] index ָ��λ���±�, ��0��ʼ
 * @return �ڵ��ַ
 */
static void* Cs_DListPeekAt(CsDListContext *context, int index)
{
    CsDListLink *node;
    index++;
    if ((index < 1) || (index > context->count))
    {
        return (NULL);
    }

    if (index < (context->count >> 1))
    {
        node = context->_CS_DLIST_HEAD;
        while (--index > 0)
        {
            node = node->next;
        }
    }
    else
    {
        index -= context->count;
        node = context->_CS_DLIST_TAIL;

        while (index++ < 0)
        {
            node = node->prev;
        }
    }

    return (node);
}

/**
 * @brief ��ȡ���ָ���ڵ�,ƫ����N��λ�õĽڵ�
 * @param [in] node ָ����׼�ڵ�λ��
 * @param [in] step ��Ի�׼�ڵ�,ƫ�ƶ��ٲ�.�������ƫ��,������ǰƫ��
 * @return ���ؽڵ�ָ��
 */
static void* Cs_DListNStep(void *node, int step)
{
    int i;

    for (i = 0; i < abs(step); i++)
    {
        if (step < 0)
        {
            node = _CS_CAST2DLIST_LINK(node)->prev;
        }
        else if (step > 0)
        {
            node = _CS_CAST2DLIST_LINK(node)->next;
        }
        if (node == NULL)
        {
            break;
        }
    }
    return (node);
}

#ifdef __cplusplus
}
#endif

#endif // CS_DLIST_H_3935E820_DD02_4DD7_AC55_9F92796BC977
