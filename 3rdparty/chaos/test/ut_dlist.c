#include "chaos/cs_dlist.h"

/**
 * @brief �����Լ��Ľڵ�
 * @note ֻ��Ҫ�ڽṹ�����ǰ��, ��һ����CS_DLIST_NODE_OBJECT, ����
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

    // 1. ��ʼ������������
    Cs_DListInit(&ctx);

    // 2. �������в���ڵ�
    for (i = 0; i < 20; i++)
    {
        DListNode* node = Cs_DListNodeMalloc(sizeof(DListNode));
        node->id = i;
        node->age = i * 10;
        Cs_DListPushBack(&ctx, node); // ���뵽����β��
    }

    assert(20 == Cs_DListGetSize(&ctx)); // ��ȡ����ڵ�ĸ���

    // 3. ����һ���½ڵ�, ������ڵ�, ���뵽��һ���ڵ�ĺ���
    nodeInsert = Cs_DListNodeMalloc(sizeof(DListNode));
    nodeInsert->id = 888;
    nodeInsert->age = 8880;
    first = Cs_DListPeekFront(&ctx);
    Cs_DListInsert(&ctx, first, nodeInsert);

    assert(21 == Cs_DListGetSize(&ctx)); // ��ȡ����ڵ�ĸ���

    // 4. ����, �ȶ�������
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

    // 5. ����ʼ��, �ͷ�������Դ
    Cs_DListUninit(&ctx);
}

void testDList2()
{
    int i = 0;
    CsDListContext ctx1;
    CsDListContext ctx2;
    DListNode* begin = NULL;
    DListNode* end = NULL;

    // ��ʼ������������
    Cs_DListInit(&ctx1);

    // �������в���ڵ�
    for (i = 0; i < 20; i++)
    {
        DListNode* node = Cs_DListNodeMalloc(sizeof(DListNode));
        node->id = i;
        node->age = i * 10;
        Cs_DListPushFront(&ctx1, node); // ��ͷ������ڵ�
    }

    begin = Cs_DListPeekAt(&ctx1, 5); // ��ȡָ���±�λ�õĽڵ�
    end = Cs_DListNStep(begin, 9); // ��ָ���ڵ㿪ʼ, ����9��, ȡ����һ���ڵ�
    Cs_DListExtract(&ctx1, begin, end, &ctx2); // ������1��ָ����������нڵ�, ����ʼ������2. �������, ��������1�б��Ƴ�. ��ӵ�����2��
    assert(10 == Cs_DListGetSize(&ctx1));
    assert(10 == Cs_DListGetSize(&ctx2));

    // ����ʼ��
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

    // ��ʼ��
    Cs_DListInit(&ctx);

    // �������в���ڵ�
    for (i = 0; i < 20; i++)
    {
        DListNode* node = Cs_DListNodeMalloc(sizeof(DListNode)); // ����һ���½ڵ�
        node->id = i;
        node->age = i * 10;
        Cs_DListPushFront(&ctx, node); // ��ͷ������ڵ�
    }

    begin = Cs_DListPeekAt(&ctx, 2); // ��ȡָ���±�λ�õĽڵ�
    end = Cs_DListNStep(begin, 10); // ��ָ���ڵ㿪ʼ, ����9��, ȡ����һ���ڵ�
    findIndex = Cs_DListFind(&ctx, end); // ����ָ���ڵ����ڵ��±�λ��
    assert(12 == findIndex);

    // ����ʼ��
    Cs_DListUninit(&ctx);
}

void testDList4()
{
    int i = 0;
    CsDListContext ctx;
    DListNode* begin = NULL;
    DListNode* end = NULL;

    // ��ʼ��
    Cs_DListInit(&ctx);

    // �������в����½ڵ�
    for (i = 0; i < 5; i++)
    {
        DListNode* node = Cs_DListNodeMalloc(sizeof(DListNode));
        node->id = 1000 + i;
        node->age = node->id * 10;
        Cs_DListPushFront(&ctx, node); // �������ͷ������
    }

    end = Cs_DListPopBack(&ctx);
    begin = Cs_DListPopFront(&ctx);
    assert(3 == Cs_DListGetSize(&ctx));

    // pop�����Ľڵ�, ��Ҫ�ֶ��ͷ�
    Cs_DListNodeFree(end);
    Cs_DListNodeFree(begin);

    // ����ʼ��
    Cs_DListUninit(&ctx);
}

void Cs_TestDList(void **state)
{
    testDList1();
    testDList2();
    testDList3();
    testDList4();
}
