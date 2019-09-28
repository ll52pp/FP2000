#include "chaos/cs_ring_buf.h"

typedef struct
{
    CS_RING_BUF_NODE_OBJECT;
    int id;
    int age;
} MyTestRingBufNode;

void testRingBuf1()
{
    int i = 0;
    CsRingBufContext ctx;
    MyTestRingBufNode* next = NULL;

    // ��ʼ��������������
    Cs_RingBufInit(&ctx);

    // �򻺳��������µĽڵ�
    for (i = 0; i < 50; i++)
    {
        MyTestRingBufNode* node = Cs_RingBufNodeMalloc(sizeof(MyTestRingBufNode));
        node->id = i;
        node->age = i * 10;
        Cs_RingBufPushBack(&ctx, node);
    }
    assert(50 == Cs_RingBufGetSize(&ctx)); // ��ȡ�������ڵ����

    // ����
    i = 0;
    next = Cs_RingBufPeekFront(&ctx);
    while (next)
    {
        assert(i == next->id);
        assert((i * 10) == next->age);
        i++;

        next = Cs_RingBufNext(next);
    }

    // ��ͣ�Ļ�����Ȧ�ƶ�
    for (i = 0; i < 200; i++)
    {
        MyTestRingBufNode* first = Cs_RingBufPeekFront(&ctx);
        assert((i % 50) == first->id);
        assert(((i % 50) * 10) == first->age);
        Cs_RingBufMove2NextNode(&ctx);
    }

    // ����ʼ��
    Cs_RingBufUninit(&ctx);
}

void testRingBuf2()
{
    int i = 0;
    CsRingBufContext ctx;

    // ��ʼ��������������
    Cs_RingBufInit(&ctx);

    // �򻺳��������µĽڵ�
    for (i = 0; i < 2; i++)
    {
        MyTestRingBufNode* node = Cs_RingBufNodeMalloc(sizeof(MyTestRingBufNode));
        node->id = i;
        node->age = i * 10;
        Cs_RingBufPushBack(&ctx, node);
    }
    assert(2 == Cs_RingBufGetSize(&ctx)); // ��ȡ�������ڵ����

    // ��ͣ�Ļ�����Ȧ�ƶ�
    for (i = 0; i < 20; i++)
    {
        MyTestRingBufNode* first = Cs_RingBufPeekFront(&ctx);
        assert((i % 2) == first->id);
        assert(((i % 2) * 10) == first->age);
        Cs_RingBufMove2NextNode(&ctx);
    }

    // ����ʼ��
    Cs_RingBufUninit(&ctx);
}

void testRingBuf3()
{
    int i = 0;
    CsRingBufContext ctx;

    // ��ʼ��������������
    Cs_RingBufInit(&ctx);

    // �򻺳��������µĽڵ�
    for (i = 0; i < 2; i++)
    {
        MyTestRingBufNode* node = Cs_RingBufNodeMalloc(sizeof(MyTestRingBufNode));
        node->id = i;
        node->age = i * 10;
        Cs_RingBufPushBack(&ctx, node);
    }
    assert(2 == Cs_RingBufGetSize(&ctx)); // ��ȡ�������ڵ����

    // ��ͣ�Ļ�����Ȧ�ƶ�
    for (i = 0; i < 20; i++)
    {
        MyTestRingBufNode* first = Cs_RingBufPeekFront(&ctx);
        assert((i % 2) == first->id);
        assert(((i % 2) * 10) == first->age);
        Cs_RingBufMove2NextNode(&ctx);
    }

    // ����ʼ��
    Cs_RingBufUninit(&ctx);
}

void testRingBuf4()
{
    int i = 0;
    CsRingBufContext ctx1;
    CsRingBufContext ctx2;
    MyTestRingBufNode* node = NULL;

    // ��ʼ��������������
    Cs_RingBufInit(&ctx1);
    Cs_RingBufInit(&ctx2);

    // �򻺳��������µĽڵ�
    for (i = 0; i < 20; i++)
    {
        MyTestRingBufNode* node = Cs_RingBufNodeMalloc(sizeof(MyTestRingBufNode));
        node->id = i;
        node->age = i * 10;
        Cs_RingBufPushBack(&ctx1, node);
    }

    // ��������1�ڵĽڵ�, ����Ƴ�, ׷�ӵ�������2��
    node = Cs_RingBufPopFront(&ctx1);
    while (node)
    {
        Cs_RingBufPushBack(&ctx2, node);
        node = Cs_RingBufPopFront(&ctx1);
    }
    assert(0 == Cs_RingBufGetSize(&ctx1));
    assert(20 == Cs_RingBufGetSize(&ctx2));

    // ��ͣ�Ļ�����Ȧ�ƶ�
    for (i = 0; i < 200; i++)
    {
        MyTestRingBufNode* first = Cs_RingBufPeekFront(&ctx2);
        assert((i % 20) == first->id);
        assert(((i % 20) * 10) == first->age);
        Cs_RingBufMove2NextNode(&ctx2);
    }

    // ��������2�ڵĽڵ�, ����Ƴ�, ׷�ӵ�������1��
    node = Cs_RingBufPopFront(&ctx2);
    while (node)
    {
        Cs_RingBufPushBack(&ctx1, node);
        node = Cs_RingBufPopFront(&ctx2);
    }

    // ��ȡ�������ڵ����
    assert(20 == Cs_RingBufGetSize(&ctx1));
    assert(0 == Cs_RingBufGetSize(&ctx2));

    // ��ͣ�Ļ�����Ȧ�ƶ�
    for (i = 0; i < 200; i++)
    {
        MyTestRingBufNode* first = Cs_RingBufPeekFront(&ctx1);
        assert((i % 20) == first->id);
        assert(((i % 20) * 10) == first->age);
        Cs_RingBufMove2NextNode(&ctx1);
    }

    // ����ʼ��
    Cs_RingBufUninit(&ctx1);
    Cs_RingBufUninit(&ctx2);
}

void Cs_TestRingBuf(void **state)
{
    testRingBuf1();
    testRingBuf2();
    testRingBuf3();
    testRingBuf4();
}
