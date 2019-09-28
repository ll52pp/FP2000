#include "chaos/cs_linked_hash_map.h"

void testLinkedHashMap1()
{
    CsLinkedHashMapContext ctx;
    Cs_LinkedHashMapInit(&ctx, 10);

    Cs_LinkedHashMapInsert(&ctx, (void*)10, (void*)20);
    Cs_LinkedHashMapInsert(&ctx, (void*)11, (void*)20);
    Cs_LinkedHashMapInsert(&ctx, (void*)10, (void*)32);
    Cs_LinkedHashMapInsert(&ctx, (void*)20, (void*)66);

    assert(32 == (int)Cs_LinkedHashMapGet(&ctx, (void*)10));
    assert(3 == Cs_LinkedHashMapGetSize(&ctx));

    Cs_LinkedHashMapErase(&ctx, (void*)11);
    assert(2 == Cs_LinkedHashMapGetSize(&ctx));
    assert(66 == (int)Cs_LinkedHashMapGet(&ctx, (void*)20));

    Cs_LinkedHashMapUninit(&ctx);
}

void testLinkedHashMap2()
{
    int i = 0;
    CsLinkedHashMapBucket* iter = NULL;
    CsLinkedHashMapContext ctx;
    Cs_LinkedHashMapInit(&ctx, 20);

    for (i = 0; i < 100; i++)
    {
        Cs_LinkedHashMapInsert(&ctx, (void*)i, (void*)(i * 10));
    }
    assert(100 == Cs_LinkedHashMapGetSize(&ctx));

    i = 0;
    // 正向遍历
    for (iter = Cs_LinkedHashMapBegin(&ctx); iter != NULL; iter = Cs_LinkedHashMapNext(iter))
    {
        assert(i == (int)iter->key);
        assert((i * 10) == (int)iter->val);
        i++;
    }

    i = 99;
    // 反向遍历
    for (iter = Cs_LinkedHashMapEnd(&ctx); iter != NULL; iter = Cs_LinkedHashMapPrevious(iter))
    {
        assert(i == (int)iter->key);
        assert((i * 10) == (int)iter->val);
        i--;
    }

    assert(1 == Cs_LinkedHashMapIsExist(&ctx, (void*)10));
    assert(100 == (int)Cs_LinkedHashMapGet(&ctx, (void*)10));
    Cs_LinkedHashMapErase(&ctx, (void*)10);
    assert(0 == Cs_LinkedHashMapIsExist(&ctx, (void*)10));
    assert(99 == Cs_LinkedHashMapGetSize(&ctx));

    Cs_LinkedHashMapUninit(&ctx);
}

void testLinkedHashMap3()
{
    int i = 0;
    CsLinkedHashMapBucket* iter = NULL;
    CsLinkedHashMapContext ctx;
    Cs_LinkedHashMapInit(&ctx, 1000);

    for (i = 0; i < 100; i++)
    {
        Cs_LinkedHashMapInsert(&ctx, (void*)i, (void*)(i * 10));
    }
    assert(100 == Cs_LinkedHashMapGetSize(&ctx));

    i = 0;
    // 正向遍历
    for (iter = Cs_LinkedHashMapBegin(&ctx); iter != NULL; iter = Cs_LinkedHashMapNext(iter))
    {
        assert(i == (int)iter->key);
        assert((i * 10) == (int)iter->val);
        i++;
    }

    i = 99;
    // 反向遍历
    for (iter = Cs_LinkedHashMapEnd(&ctx); iter != NULL; iter = Cs_LinkedHashMapPrevious(iter))
    {
        assert(i == (int)iter->key);
        assert((i * 10) == (int)iter->val);
        i--;
    }

    for (i = 0; i < 10000; i++)
    {
        Cs_LinkedHashMapInsert(&ctx, (void*)i, (void*)(i * 100));
    }
    assert(10000 == Cs_LinkedHashMapGetSize(&ctx));

    i = 0;
    // 正向遍历
    for (iter = Cs_LinkedHashMapBegin(&ctx); iter != NULL; iter = Cs_LinkedHashMapNext(iter))
    {
        assert(i == (int)iter->key);
        assert((i * 100) == (int)iter->val);
        i++;
    }

    i = 9999;
    // 反向遍历
    for (iter = Cs_LinkedHashMapEnd(&ctx); iter != NULL; iter = Cs_LinkedHashMapPrevious(iter))
    {
        assert(i == (int)iter->key);
        assert((i * 100) == (int)iter->val);
        i--;
    }

    assert(1 == Cs_LinkedHashMapIsExist(&ctx, (void*)10));
    assert(1000 == (int)Cs_LinkedHashMapGet(&ctx, (void*)10));
    Cs_LinkedHashMapErase(&ctx, (void*)10);
    assert(0 == Cs_LinkedHashMapIsExist(&ctx, (void*)10));
    assert(9999 == Cs_LinkedHashMapGetSize(&ctx));

    Cs_LinkedHashMapUninit(&ctx);
}

void testLinkedHashMap4()
{
    int i = 0;
    CsLinkedHashMapBucket* bucket = NULL;
    CsLinkedHashMapContext ctx;
    Cs_LinkedHashMapInit(&ctx, 2);

    for (i = 0; i < 100; i++)
    {
        Cs_LinkedHashMapInsert(&ctx, (void*)i, (void*)(i * 10));
    }
    assert(100 == Cs_LinkedHashMapGetSize(&ctx));

    Cs_LinkedHashMapErase(&ctx, (void*)55);
    assert(0 == Cs_LinkedHashMapIsExist(&ctx, (void*)55));
    Cs_LinkedHashMapInsert(&ctx, (void*)55, (void*)3);
    assert(1 == Cs_LinkedHashMapIsExist(&ctx, (void*)55));
    assert(3 == (int)Cs_LinkedHashMapGet(&ctx, (void*)55));

    bucket = Cs_LinkedHashMapEnd(&ctx);
    assert(bucket->key == (void*)55);
    assert(bucket->val == (void*)3);

    bucket = Cs_LinkedHashMapPrevious(bucket);
    assert(bucket->key == (void*)99);
    assert(bucket->val == (void*)990);
}

void Cs_TestLinkedHashMap(void **state)
{
    testLinkedHashMap1();
    testLinkedHashMap2();
    testLinkedHashMap3();
    testLinkedHashMap4();
}
