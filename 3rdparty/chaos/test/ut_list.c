#include "ut_header.h"
#include "chaos/cs_list.h"

/**
 * @brief 定义自己的节点
 * @note 只需要在结构体的最前面, 加一个宏CS_LIST_NODE_OBJECT, 即可
 */
typedef struct
{
    CS_LIST_NODE_OBJECT;
    int id;
    int age;
} ListNode;

void testList1()
{
    int i = 0;
    CsListContext ctx;
    ListNode* next = NULL;

    // 1. 初始化链表上下文
    Cs_ListInit(&ctx);

    // 2. 向链表中插入节点
    for (i = 0; i < 20; i++)
    {
        ListNode* node = (ListNode*)Cs_ListNodeMalloc(sizeof(ListNode));
        node->id = i;
        node->age = i * 10;
        Cs_ListPushBack(&ctx, node); // 插入到链表尾部
    }

    // 3. 获取链接节点的个数
    assert_int_equal(20, Cs_ListGetSize(&ctx));

    // 4. 遍历链表
    next = Cs_ListPeekFront(&ctx);
    i = 0;
    while (next)
    {
        assert_int_equal(i, next->id);
        assert_int_equal((i * 10), next->age);
        i++;

        next = Cs_ListNext(next);
    }

    // 5. 反初始化
    Cs_ListUninit(&ctx);
}

void testList2()
{
    int i = 0;
    CsListContext ctx;
    ListNode* front = NULL;

    // 1. 初始化链表上下文
    Cs_ListInit(&ctx);

    // 2. 向链表中插入节点
    for (i = 0; i < 20; i++)
    {
        ListNode* node = (ListNode*)Cs_ListNodeMalloc(sizeof(ListNode));
        node->id = i;
        node->age = i * 10;
        Cs_ListPushFront(&ctx, node); // 插入到链表头部
    }

    // 3. 获取链接节点的个数
    assert_int_equal(20, Cs_ListGetSize(&ctx));

    // 4. 遍历链表
    for (i = 19; i >= 0; i--)
    {
        front = Cs_ListPopFront(&ctx); // 弹出链表的头节点
        assert_int_equal(i, front->id);
        assert_int_equal((i * 10), front->age);
        Cs_ListNodeFree(front); // 释放被弹出的节点
    }

    assert_int_equal(0, Cs_ListGetSize(&ctx)); // 获取链表的大小

    // 5. 反初始化
    Cs_ListUninit(&ctx);
}

void Cs_TestList(void **state)
{
    testList1();
    testList2();
}
