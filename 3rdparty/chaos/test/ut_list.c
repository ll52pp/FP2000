#include "ut_header.h"
#include "chaos/cs_list.h"

/**
 * @brief �����Լ��Ľڵ�
 * @note ֻ��Ҫ�ڽṹ�����ǰ��, ��һ����CS_LIST_NODE_OBJECT, ����
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

    // 1. ��ʼ������������
    Cs_ListInit(&ctx);

    // 2. �������в���ڵ�
    for (i = 0; i < 20; i++)
    {
        ListNode* node = (ListNode*)Cs_ListNodeMalloc(sizeof(ListNode));
        node->id = i;
        node->age = i * 10;
        Cs_ListPushBack(&ctx, node); // ���뵽����β��
    }

    // 3. ��ȡ���ӽڵ�ĸ���
    assert_int_equal(20, Cs_ListGetSize(&ctx));

    // 4. ��������
    next = Cs_ListPeekFront(&ctx);
    i = 0;
    while (next)
    {
        assert_int_equal(i, next->id);
        assert_int_equal((i * 10), next->age);
        i++;

        next = Cs_ListNext(next);
    }

    // 5. ����ʼ��
    Cs_ListUninit(&ctx);
}

void testList2()
{
    int i = 0;
    CsListContext ctx;
    ListNode* front = NULL;

    // 1. ��ʼ������������
    Cs_ListInit(&ctx);

    // 2. �������в���ڵ�
    for (i = 0; i < 20; i++)
    {
        ListNode* node = (ListNode*)Cs_ListNodeMalloc(sizeof(ListNode));
        node->id = i;
        node->age = i * 10;
        Cs_ListPushFront(&ctx, node); // ���뵽����ͷ��
    }

    // 3. ��ȡ���ӽڵ�ĸ���
    assert_int_equal(20, Cs_ListGetSize(&ctx));

    // 4. ��������
    for (i = 19; i >= 0; i--)
    {
        front = Cs_ListPopFront(&ctx); // ���������ͷ�ڵ�
        assert_int_equal(i, front->id);
        assert_int_equal((i * 10), front->age);
        Cs_ListNodeFree(front); // �ͷű������Ľڵ�
    }

    assert_int_equal(0, Cs_ListGetSize(&ctx)); // ��ȡ����Ĵ�С

    // 5. ����ʼ��
    Cs_ListUninit(&ctx);
}

void Cs_TestList(void **state)
{
    testList1();
    testList2();
}
