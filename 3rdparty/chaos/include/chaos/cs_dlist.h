/**
 * @file
 * @brief 双向环形链表
 *
 * 适用于首尾大量增删改操作, 中间插入操作
 * @author yaoyuliang(765495939@qq.com)
 *
 * 链表数据结构:
 *
 *```
 *   1. 空链表
 *         -----------
 *         |  head------------------
 *         |         |             |
 *         |  tail----------       |
 *         |         |     |       v
 *         | count=0 |   -----   -----
 *         -----------    ---     ---
 *                         -       -
 *
 *   2. 非空链表
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
    CsDListLink link; ///< 指向下一节点
    int         count; ///< 节点个数
} CsDListContext;

#define CS_DLIST_NODE_OBJECT CsDListLink _csDLink ///< 你自定义的节点结构体, 必须在成员变量的最前面加上这个宏

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
// 以下是函数实现部分...

/**
 * @brief 初始化
 * @param [in] context 上下文
 */
static void Cs_DListInit(CsDListContext *context)
{
    assert(NULL != context);
    context->_CS_DLIST_HEAD = NULL;
    context->_CS_DLIST_TAIL = NULL;
    context->count = 0;
}

/**
 * @brief 反初始化
 * @param [in] context 上下文
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
 * @brief 申请一个指定大小的链表节点
 * @param [in] nodeSize 节点大小
 * @return 返回申请到的节点的地址
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
 * @brief 向链表的指定节点位置后方, 插入一个新节点
 * @param [in] context 上下文
 * @param [in] prev 指定要被插入的节点
 * @param [in] node 新的待被插入的节点
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
 * @brief 在链表头部插入新节点
 * @param [in] context 上下文
 * @param [in] node 待插入的新节点
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
 * @brief 向链表尾部插入新节点
 * @param [in] context 上下文
 * @param [in] node 待插入的新节点
 */
static void Cs_DListPushBack(CsDListContext *context, void *node)
{
    assert(NULL != context);
    assert(NULL != node);
    Cs_DListInsert(context, context->_CS_DLIST_TAIL, node);
}

/**
 * @brief 弹出头节点
 * @param [in] context 上下文
 * @return 返回节点地址
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
 * @brief 弹出尾节点
 * @param [in] context 上下文
 * @return 返回节点地址
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
 * @brief 从链表中移除指定的节点, 不会释放节点
 * @param [in] context 上下文
 * @param [in] node 要移除的节点
 * @attention 只是从链表中移除, 不会释放节点占用的内存, 注意别内存泄漏
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
 * @brief 将2个链表, 合并成一个链表
 * @param [out] dstContext 目标链表
 * @param [in] srcContext 源链表
 * @attention 因为不涉及节点的拷贝, 而是直接将源链表的所有节点, 指向目标链表尾部, 所以源链表被合入后, 将变成空链表
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
 * @brief 用源链表中的一段, 来初始化目标链表
 * @param [in] srcContext 源链表上下文
 * @param [in] startNode 源链表中要被提取的起始节点
 * @param [in] endNode 源链表中要被提取的结束节点
 * @param [out] dstContext 目标链表, 必须为空链表, 或者没初始化过
 * @attention 是用源链表指定的一段, 来初始化目标链表的, 所以目标链表, 必须为未初始的
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
 * @brief 查找指定节点所在的位置下标
 * @param [in] context 上下文
 * @param [in] node 要查找的节点
 * @return 节点在链表中的位置下标
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
 * @brief 取得指定位置下标的节点
 * @param [in] context 上下文
 * @param [in] index 指定位置下标, 从0开始
 * @return 节点地址
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
 * @brief 获取相对指定节点,偏移了N个位置的节点
 * @param [in] node 指定基准节点位置
 * @param [in] step 相对基准节点,偏移多少步.正数向后偏移,负数向前偏移
 * @return 返回节点指针
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
