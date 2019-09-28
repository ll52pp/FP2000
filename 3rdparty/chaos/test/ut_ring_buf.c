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

    // 初始化缓冲区上下文
    Cs_RingBufInit(&ctx);

    // 向缓冲区插入新的节点
    for (i = 0; i < 50; i++)
    {
        MyTestRingBufNode* node = Cs_RingBufNodeMalloc(sizeof(MyTestRingBufNode));
        node->id = i;
        node->age = i * 10;
        Cs_RingBufPushBack(&ctx, node);
    }
    assert(50 == Cs_RingBufGetSize(&ctx)); // 获取缓冲区节点个数

    // 遍历
    i = 0;
    next = Cs_RingBufPeekFront(&ctx);
    while (next)
    {
        assert(i == next->id);
        assert((i * 10) == next->age);
        i++;

        next = Cs_RingBufNext(next);
    }

    // 不停的环形绕圈移动
    for (i = 0; i < 200; i++)
    {
        MyTestRingBufNode* first = Cs_RingBufPeekFront(&ctx);
        assert((i % 50) == first->id);
        assert(((i % 50) * 10) == first->age);
        Cs_RingBufMove2NextNode(&ctx);
    }

    // 反初始化
    Cs_RingBufUninit(&ctx);
}

void testRingBuf2()
{
    int i = 0;
    CsRingBufContext ctx;

    // 初始化缓冲区上下文
    Cs_RingBufInit(&ctx);

    // 向缓冲区插入新的节点
    for (i = 0; i < 2; i++)
    {
        MyTestRingBufNode* node = Cs_RingBufNodeMalloc(sizeof(MyTestRingBufNode));
        node->id = i;
        node->age = i * 10;
        Cs_RingBufPushBack(&ctx, node);
    }
    assert(2 == Cs_RingBufGetSize(&ctx)); // 获取缓冲区节点个数

    // 不停的环形绕圈移动
    for (i = 0; i < 20; i++)
    {
        MyTestRingBufNode* first = Cs_RingBufPeekFront(&ctx);
        assert((i % 2) == first->id);
        assert(((i % 2) * 10) == first->age);
        Cs_RingBufMove2NextNode(&ctx);
    }

    // 反初始化
    Cs_RingBufUninit(&ctx);
}

void testRingBuf3()
{
    int i = 0;
    CsRingBufContext ctx;

    // 初始化缓冲区上下文
    Cs_RingBufInit(&ctx);

    // 向缓冲区插入新的节点
    for (i = 0; i < 2; i++)
    {
        MyTestRingBufNode* node = Cs_RingBufNodeMalloc(sizeof(MyTestRingBufNode));
        node->id = i;
        node->age = i * 10;
        Cs_RingBufPushBack(&ctx, node);
    }
    assert(2 == Cs_RingBufGetSize(&ctx)); // 获取缓冲区节点个数

    // 不停的环形绕圈移动
    for (i = 0; i < 20; i++)
    {
        MyTestRingBufNode* first = Cs_RingBufPeekFront(&ctx);
        assert((i % 2) == first->id);
        assert(((i % 2) * 10) == first->age);
        Cs_RingBufMove2NextNode(&ctx);
    }

    // 反初始化
    Cs_RingBufUninit(&ctx);
}

void testRingBuf4()
{
    int i = 0;
    CsRingBufContext ctx1;
    CsRingBufContext ctx2;
    MyTestRingBufNode* node = NULL;

    // 初始化缓冲区上下文
    Cs_RingBufInit(&ctx1);
    Cs_RingBufInit(&ctx2);

    // 向缓冲区插入新的节点
    for (i = 0; i < 20; i++)
    {
        MyTestRingBufNode* node = Cs_RingBufNodeMalloc(sizeof(MyTestRingBufNode));
        node->id = i;
        node->age = i * 10;
        Cs_RingBufPushBack(&ctx1, node);
    }

    // 将缓冲区1内的节点, 逐个移除, 追加到缓冲区2内
    node = Cs_RingBufPopFront(&ctx1);
    while (node)
    {
        Cs_RingBufPushBack(&ctx2, node);
        node = Cs_RingBufPopFront(&ctx1);
    }
    assert(0 == Cs_RingBufGetSize(&ctx1));
    assert(20 == Cs_RingBufGetSize(&ctx2));

    // 不停的环形绕圈移动
    for (i = 0; i < 200; i++)
    {
        MyTestRingBufNode* first = Cs_RingBufPeekFront(&ctx2);
        assert((i % 20) == first->id);
        assert(((i % 20) * 10) == first->age);
        Cs_RingBufMove2NextNode(&ctx2);
    }

    // 将缓冲区2内的节点, 逐个移除, 追加到缓冲区1内
    node = Cs_RingBufPopFront(&ctx2);
    while (node)
    {
        Cs_RingBufPushBack(&ctx1, node);
        node = Cs_RingBufPopFront(&ctx2);
    }

    // 获取缓冲区节点个数
    assert(20 == Cs_RingBufGetSize(&ctx1));
    assert(0 == Cs_RingBufGetSize(&ctx2));

    // 不停的环形绕圈移动
    for (i = 0; i < 200; i++)
    {
        MyTestRingBufNode* first = Cs_RingBufPeekFront(&ctx1);
        assert((i % 20) == first->id);
        assert(((i % 20) * 10) == first->age);
        Cs_RingBufMove2NextNode(&ctx1);
    }

    // 反初始化
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
