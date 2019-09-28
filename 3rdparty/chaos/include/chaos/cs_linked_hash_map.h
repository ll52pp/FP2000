/**
 * @file 哈希表
 */

#ifndef CS_HASH_MAP_H_80A595AD_CC31_4BD3_8FE5_A8432889D71C
#define CS_HASH_MAP_H_80A595AD_CC31_4BD3_8FE5_A8432889D71C

#include "cs_base.h"

typedef size_t(*Cs_LinkedHashMapHashFuncType)(const void* input);
typedef unsigned int(*Cs_LinkedHashMapIsKeyEqualFuncType)(const void* lhs, const void* rhs);

typedef struct CsLinkedHashMapBucketTag
{
    void* key;      ///< 键
    void* val;      ///< 值
    int   isUsed;   ///< 1表示该桶已经存有数据, 0表示该桶为空,未被使用
    struct CsLinkedHashMapBucketTag* hashNext;
    struct CsLinkedHashMapBucketTag* listNext;
    struct CsLinkedHashMapBucketTag* listPrev;
} CsLinkedHashMapBucket;

typedef struct
{
    Cs_LinkedHashMapHashFuncType       pfnHashCode;     ///< hash函数
    Cs_LinkedHashMapIsKeyEqualFuncType pfnIsKeyEqual;   ///< 值比较函数
    int arrSize; ///< 桶个数
    int elemNo;  ///< 元素个数
    CsLinkedHashMapBucket* buckets;
    CsLinkedHashMapBucket* listHead;
    CsLinkedHashMapBucket* listTail;
} CsLinkedHashMapContext;

#define Cs_LinkedHashMapBegin(context) ((context)->listHead)
#define Cs_LinkedHashMapEnd(context) ((context)->listTail)
#define Cs_LinkedHashMapNext(bucket) ((bucket)->listNext)
#define Cs_LinkedHashMapPrevious(bucket) ((bucket)->listPrev)
#define Cs_LinkedHashMapGetSize(context) ((context)->elemNo)

// 函数在此声明...
static void  Cs_LinkedHashMapInit(CsLinkedHashMapContext* ctx, size_t arraySize);
static void  Cs_LinkedHashMapSetHashFunc(CsLinkedHashMapContext* ctx, Cs_LinkedHashMapHashFuncType pfnHashCode);
static void  Cs_LinkedHashMapSetCompareFunc(CsLinkedHashMapContext* ctx, Cs_LinkedHashMapIsKeyEqualFuncType pfnIsKeyEqual);
static void  Cs_LinkedHashMapUninit(CsLinkedHashMapContext* ctx);
static void  Cs_LinkedHashMapInsert(CsLinkedHashMapContext* ctx, void* key, void* value);
static int   Cs_LinkedHashMapIsExist(CsLinkedHashMapContext* ctx, const void* key);
static void* Cs_LinkedHashMapGet(CsLinkedHashMapContext* ctx, const void* key);
static void  Cs_LinkedHashMapErase(CsLinkedHashMapContext* ctx, void* key);

// 提供几个默认的hash值算法
static size_t Cs_LinkedHashMapHashIntergerHashCode(const void* input);
static size_t Cs_LinkedHashMapBkdrHashCode(const void *input); // key为字符串时, 首推荐
static size_t Cs_LinkedHashMapElfHashCode(const void *input); // key为字符串时, 次推荐
static size_t Cs_LinkedHashMapFnvHashCode(const void *input);

// 提供几个默认的key对比函数
static unsigned int Cs_LinkedHashMapIsEquls(const void* lhs, const void* rhs);

// 私有函数
static void _Cs_LinkedHashMapFreeBuckets(CsLinkedHashMapContext* ctx, CsLinkedHashMapBucket* head);
static void _Cs_LinkedHashMapListInsert(CsLinkedHashMapContext* ctx, CsLinkedHashMapBucket* prev, CsLinkedHashMapBucket* node);
static void _Cs_LinkedHashMapListPushBack(CsLinkedHashMapContext* ctx, CsLinkedHashMapBucket* node);
static int  _Cs_HashCode(CsLinkedHashMapContext* ctx, const void* key);
static void _Cs_LinkedHashMapListErase(CsLinkedHashMapContext *context, CsLinkedHashMapBucket *node);
static CsLinkedHashMapBucket* _Cs_HashmapFindBucket(CsLinkedHashMapBucket* head, const void* key);

// 以下是函数实现部分...

static size_t Cs_LinkedHashMapHashIntergerHashCode(const void* input)
{
    return (size_t)input;
}

static void _Cs_LinkedHashMapFreeBuckets(CsLinkedHashMapContext* ctx, CsLinkedHashMapBucket* head)
{
    CsLinkedHashMapBucket* next = NULL;
    while (head)
    {
        next = head->hashNext;
        Cs_Free(head);
        head = next;
    }
}

static void _Cs_LinkedHashMapListInsert(CsLinkedHashMapContext* ctx, CsLinkedHashMapBucket* prev, CsLinkedHashMapBucket* node)
{
    CsLinkedHashMapBucket *next;

    if (prev == NULL)
    {
        next = ctx->listHead;
        ctx->listHead = node;
    }
    else
    {
        next = prev->listNext;
        prev->listNext = node;
    }

    if (next == NULL)
    {
        ctx->listTail = node;
    }
    else
    {
        next->listPrev = node;
    }

    node->listNext = next;
    node->listPrev = prev;
}

static void _Cs_LinkedHashMapListPushBack(CsLinkedHashMapContext* ctx, CsLinkedHashMapBucket* node)
{
    _Cs_LinkedHashMapListInsert(ctx, ctx->listTail, node);
}

static int _Cs_HashCode(CsLinkedHashMapContext* ctx, const void* key)
{
    return ctx->pfnHashCode(key) % ctx->arrSize;
}

static CsLinkedHashMapBucket* _Cs_HashmapFindBucket(CsLinkedHashMapBucket* head, const void* key)
{
    while (head)
    {
        if (key == head->key)
        {
            return head;
        }
        head = head->hashNext;
    }
    return NULL;
}

static void _Cs_LinkedHashMapListErase(CsLinkedHashMapContext *context, CsLinkedHashMapBucket *node)
{
    if (node->listPrev == NULL)
    {
        context->listHead = node->listNext;
    }
    else
    {
        node->listPrev->listNext = node->listNext;
    }

    if (node->listNext == NULL)
    {
        context->listTail = node->listPrev;
    }
    else
    {
        node->listNext->listPrev = node->listPrev;
    }
}

static size_t Cs_LinkedHashMapBkdrHashCode(const void *input) // key为字符串时, 首推荐
{
    const char* buf = (const char*)input;
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    while (*buf)
    {
        hash = hash * seed + (*buf++);
    }
    return hash & 0x7fffffff;
}

static size_t Cs_LinkedHashMapElfHashCode(const void *input) // key为字符串时, 次推荐
{
    const char* buf = (const char*)input;
    unsigned int hash = 0;
    unsigned int x = 0;
    while (*buf)
    {
        hash = (hash << 4) + (*buf++);
        if ((x = hash & 0xf0000000l) != 0)
        {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }
    return hash & 0x7fffffff;
}

static size_t Cs_LinkedHashMapFnvHashCode(const void *input)
{
    size_t n = (size_t)input;
    char* chArr = (char*)&n;
    size_t hash = (size_t)2166136261UL;
    int i = 0;
    for (; i < sizeof(size_t); i++)
    {
        hash = (hash * 16777619) ^ chArr[i];
    }

    return hash;
}

// 提供几个默认的key对比函数

static unsigned int Cs_LinkedHashMapIsEquls(const void* lhs, const void* rhs)
{
    return (lhs == rhs ? 1 : 0);
}

static void Cs_LinkedHashMapInit(CsLinkedHashMapContext* ctx, size_t arraySize)
{
    ctx->arrSize = arraySize;
    ctx->elemNo = 0;
    ctx->listHead = NULL;
    ctx->listTail = NULL;
    ctx->pfnHashCode = Cs_LinkedHashMapHashIntergerHashCode;
    ctx->pfnIsKeyEqual = Cs_LinkedHashMapIsEquls;
    ctx->buckets = (CsLinkedHashMapBucket*)Cs_Malloc(sizeof(CsLinkedHashMapBucket) * arraySize);
    memset(ctx->buckets, 0, sizeof(CsLinkedHashMapBucket) * arraySize);
}

static void Cs_LinkedHashMapSetHashFunc(CsLinkedHashMapContext* ctx, Cs_LinkedHashMapHashFuncType pfnHashCode)
{
    ctx->pfnHashCode = pfnHashCode;
}

static void Cs_LinkedHashMapSetCompareFunc(CsLinkedHashMapContext* ctx, Cs_LinkedHashMapIsKeyEqualFuncType pfnIsKeyEqual)
{
    ctx->pfnIsKeyEqual = pfnIsKeyEqual;
}

static void Cs_LinkedHashMapUninit(CsLinkedHashMapContext* ctx)
{
    int i = 0;
    for (i = 0; i < ctx->arrSize; i++)
    {
        _Cs_LinkedHashMapFreeBuckets(ctx, ctx->buckets[i].hashNext);
    }
    Cs_Free(ctx->buckets);
    ctx->buckets = NULL;
    ctx->arrSize = 0;
    ctx->elemNo = 0;
    ctx->pfnHashCode = 0;
    ctx->listHead = NULL;
    ctx->listTail = NULL;
}

static void Cs_LinkedHashMapInsert(CsLinkedHashMapContext* ctx, void* key, void* value)
{
    CsLinkedHashMapBucket* newNode = NULL;
    CsLinkedHashMapBucket* bucket = &ctx->buckets[_Cs_HashCode(ctx, key)];

    if (!bucket->isUsed)
    {
        bucket->key = key;
        bucket->val = value;
        bucket->isUsed = 1;
        _Cs_LinkedHashMapListPushBack(ctx, bucket);
        ctx->elemNo++;
        goto Exit0;
    }

    // 相等直接覆盖
    if (ctx->pfnIsKeyEqual(bucket->key, key))
    {
        bucket->val = value;
        goto Exit0;
    }

    // 遍历链表, 若有相等, 则覆盖.
    while (NULL != bucket->hashNext)
    {
        bucket = bucket->hashNext;
        if (ctx->pfnIsKeyEqual(bucket->key, key))
        {
            bucket->val = value;
            goto Exit0;
        }
    }

    // 新桶
    newNode = Cs_Malloc(sizeof(CsLinkedHashMapBucket));
    newNode->key = key;
    newNode->val = value;
    newNode->hashNext = NULL;
    newNode->isUsed = 1;
    bucket->hashNext = newNode;
    _Cs_LinkedHashMapListPushBack(ctx, newNode);
    ctx->elemNo++;

Exit0:
    return;
}

static int Cs_LinkedHashMapIsExist(CsLinkedHashMapContext* ctx, const void* key)
{
    CsLinkedHashMapBucket* bucket = &ctx->buckets[_Cs_HashCode(ctx, key)];
    CsLinkedHashMapBucket* f = _Cs_HashmapFindBucket(bucket, key);
    if (f)
    {
        return 1;
    }
    return 0;
}

static void* Cs_LinkedHashMapGet(CsLinkedHashMapContext* ctx, const void* key)
{
    CsLinkedHashMapBucket* bucket = &ctx->buckets[_Cs_HashCode(ctx, key)];
    CsLinkedHashMapBucket* f = _Cs_HashmapFindBucket(bucket, key);
    if (f)
    {
        return f->val;
    }
    return NULL;
}

static void Cs_LinkedHashMapErase(CsLinkedHashMapContext* ctx, void* key)
{
    CsLinkedHashMapBucket *bucket = &ctx->buckets[_Cs_HashCode(ctx, key)];
    if (key == bucket->key)
    {
        if (bucket->hashNext)
        {
            CsLinkedHashMapBucket* tmp = bucket->hashNext;
            bucket->key = tmp->key;
            bucket->val = tmp->val;
            bucket->hashNext = tmp->hashNext;
            _Cs_LinkedHashMapListErase(ctx, tmp);

            Cs_Free(tmp);
            ctx->elemNo--;
            goto Exit0;
        }
        else
        {
            _Cs_LinkedHashMapListErase(ctx, bucket);
            bucket->key = NULL;
            ctx->elemNo--;
            goto Exit0;
        }
    }
    else
    {
        CsLinkedHashMapBucket* next = bucket->hashNext;
        while (next)
        {
            if (key == next->key)
            {
                bucket->hashNext = next->hashNext;
                _Cs_LinkedHashMapListErase(ctx, next);
                Cs_Free(next);
                ctx->elemNo--;
                goto Exit0;
            }
            bucket = next;
            next = bucket->hashNext;
        }
    }

Exit0:
    return;
}

#endif
