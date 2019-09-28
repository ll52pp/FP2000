#include "chaos/cs_dlist.h"

/**
 * @brief 定义自己的节点
 * @note 只需要在结构体的最前面, 加一个宏CS_DLIST_NODE_OBJECT, 即可
 */
typedef struct
{
    CS_DLIST_NODE_OBJECT;
    int id;
    int age;
} DListNode;

void testDList1()
{
    DListNode* first = NULL;
    DListNode* next = NULL;
    DListNode* nodeInsert = NULL;
    int i = 0;
    CsDListContext ctx;

    // 1. 初始化链表上下文
    Cs_DListInit(&ctx);

    // 2. 向链表中插入节点
    for (i = 0; i < 20; i++)
    {
        DListNode* node = Cs_DListNodeMalloc(sizeof(DListNode));
        node->id = i;
        node->age = i * 10;
        Cs_DListPushBack(&ctx, node); // 插入到链表尾部
    }

    assert(20 == Cs_DListGetSize(&ctx)); // 获取链表节点的个数

    // 3. 创建一个新节点, 将这个节点, 插入到第一个节点的后面
    nodeInsert = Cs_DListNodeMalloc(sizeof(DListNode));
    nodeInsert->id = 888;
    nodeInsert->age = 8880;
    first = Cs_DListPeekFront(&ctx);
    Cs_DListInsert(&ctx, first, nodeInsert);

    assert(21 == Cs_DListGetSize(&ctx)); // 获取链表节点的个数

    // 4. 遍历, 比对下数据
    i = 0;
    next = Cs_DListPeekFront(&ctx);
    while (NULL != next)
    {
        if (0 == i)
        {
            assert(0 == next->id);
            assert(0 == next->age);
        }
        else if (1 == i)
        {
            assert(888 == next->id);
            assert(8880 == next->age);
        }
        else
        {
            assert((i - 1) == next->id);
            assert(((i - 1) * 10) == next->age);
        }
        i++;

        next = (DListNode*)Cs_DListNext(next);
    }

    // 5. 反初始化, 释放链表资源
    Cs_DListUninit(&ctx);
}

void testDList2()
{
    int i = 0;
    CsDListContext ctx1;
    CsDListContext ctx2;
    DListNode* begin = NULL;
    DListNode* end = NULL;

    // 初始化链表上下文
    Cs_DListInit(&ctx1);

    // 往链表中插入节点
    for (i = 0; i < 20; i++)
    {
        DListNode* node = Cs_DListNodeMalloc(sizeof(DListNode));
        node->id = i;
        node->age = i * 10;
        Cs_DListPushFront(&ctx1, node); // 从头部插入节点
    }

    begin = Cs_DListPeekAt(&ctx1, 5); // 获取指定下标位置的节点
    end = Cs_DListNStep(begin, 9); // 从指定节点开始, 向后跨9步, 取得另一个节点
    Cs_DListExtract(&ctx1, begin, end, &ctx2); // 用链表1的指定区间的所有节点, 来初始化链表2. 这块区间, 将从链表1中被移除. 添加到链表2中
    assert(10 == Cs_DListGetSize(&ctx1));
    assert(10 == Cs_DListGetSize(&ctx2));

    // 反初始化
    Cs_DListUninit(&ctx2);
    Cs_DListUninit(&ctx1);
}

void testDList3()
{
    int i = 0;
    CsDListContext ctx;
    int findIndex = -1;
    DListNode* begin = NULL;
    DListNode* end = NULL;

    // 初始化
    Cs_DListInit(&ctx);

    // 往链表中插入节点
    for (i = 0; i < 20; i++)
    {
        DListNode* node = Cs_DListNodeMalloc(sizeof(DListNode)); // 创建一个新节点
        node->id = i;
        node->age = i * 10;
        Cs_DListPushFront(&ctx, node); // 从头部插入节点
    }

    begin = Cs_DListPeekAt(&ctx, 2); // 获取指定下标位置的节点
    end = Cs_DListNStep(begin, 10); // 从指定节点开始, 向后跨9步, 取得另一个节点
    findIndex = Cs_DListFind(&ctx, end); // 查找指定节点所在的下标位置
    assert(12 == findIndex);

    // 反初始化
    Cs_DListUninit(&ctx);
}

void testDList4()
{
    int i = 0;
    CsDListContext ctx;
    DListNode* begin = NULL;
    DListNode* end = NULL;

    // 初始化
    Cs_DListInit(&ctx);

    // 向链表中插入新节点
    for (i = 0; i < 5; i++)
    {
        DListNode* node = Cs_DListNodeMalloc(sizeof(DListNode));
        node->id = 1000 + i;
        node->age = node->id * 10;
        Cs_DListPushFront(&ctx, node); // 从链表的头部插入
    }

    end = Cs_DListPopBack(&ctx);
    begin = Cs_DListPopFront(&ctx);
    assert(3 == Cs_DListGetSize(&ctx));

    // pop出来的节点, 需要手动释放
    Cs_DListNodeFree(end);
    Cs_DListNodeFree(begin);

    // 反初始化
    Cs_DListUninit(&ctx);
}

void Cs_TestDList(void **state)
{
    testDList1();
    testDList2();
    testDList3();
    testDList4();
}
