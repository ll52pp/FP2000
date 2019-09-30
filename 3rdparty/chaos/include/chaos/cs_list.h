/**
 * @file
 * @brief 单向链表
 *
 * 适用于头部大量增加,删除操作, 尾部只支持增加操作, 中间插入操作
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
    struct CsListLinkTag *next; ///< 指向下一节点
} CsListLink;

typedef struct
{
    struct CsListLinkTag*   head;  ///< 头
    struct CsListLinkTag*   tail;  ///< 尾
    int                     count; ///< 节点个数
} CsListContext;

#define CS_LIST_NODE_OBJECT CsListLink _csLink ///< 你自定义的节点结构体, 必须在成员变量的最前面加上这个宏
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
 * @brief 释放节点
 * @param [in] context 上下文
 * @param [in] node 节点首地址
 */
#define Cs_ListNodeFree(node) Cs_Free(node)


//////////////////////////////////////////////////////////////////////////
// 以下是函数实现部分...

/**
 * @brief 初始化
 * @param [in] context 上下文
 */
static void Cs_ListInit(CsListContext *context)
{
    context->head = NULL;
    context->tail = NULL;
    context->count = 0;
}
/**
 * @brief 反初始化链表, 释放所有链表上的节点
 * @param [in] context 上下文
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
 * @brief 创建一个新节点
 * @param [in] nodeSize 节点大小
 * @return 节点首地址
 */
static void* Cs_ListNodeMalloc(unsigned int nodeSize)
{
    CsListLink* link = (CsListLink*)Cs_Malloc(nodeSize);
    link->next = NULL;
    return link;
}

/**
 * @brief 在后方插入节点
 * @param [in] context 链表context
 * @param [in] prev 需要被插入的位置, 将在这个节点的后方插入
 * @param [in] node 待插入进链表的节点
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
 * @brief 将节点追加到链表的尾部
 * @param [in] context 上下文
 * @param [in] node 节点首地址
 */
static void Cs_ListPushBack(CsListContext *context, void *node)
{
    Cs_ListInsert(context, context->tail, node);
}

/**
 * @brief 将节点插入链表的头部
 * @param [in] context 上下文
 * @param [in] node 节点首地址
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
 * @brief 弹出链表的头节点, 即移除第一个节点
 * @param [in] context 上下文
 * @return 弹出的头节点的地址
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
 * @brief 获取链表节点的个数
 * @param [in] context 上下文
 * @return 节点的个数
 */
static int Cs_ListGetSize(CsListContext *context)
{
    return (context->count);
}

/**
 * @brief 获取指定节点对应的下一个节点地址
 * @param [in] node 指定节点的首地址
 * @return 下一个节点的首地址
 */
static void* Cs_ListNext(void *node)
{
    return (_CS_CAST2LIST_NODE(node)->next);
}

/**
 * @brief 获取链表的头节点
 * @param [in] context 上下文
 * @attention 注意, peek和pop的区别在于, pop会将节点从链表中移除, 而peek不会
 * @return 头节点首地址
 */
static void* Cs_ListPeekFront(CsListContext *context)
{
    return (context->head);
}

#ifdef __cplusplus
}
#endif

#endif
