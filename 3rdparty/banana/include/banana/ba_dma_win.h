#pragma once

#include "ba_base_types.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、发货 发的
typedef struct
{
    void*           virtualAddr;
    LARGE_INTEGER   logicalAddr;
    // ------------------------------------------------
} BaDmaBuf;

static bool Ba_DmaAlloc(const size_t size, const size_t alignBytes, BaDmaBuf* dma)
{
    assert(0 != size);
    assert(NULL != dma);

    dma->virtualAddr = malloc(size);
    dma->logicalAddr.LowPart = (ULONG)dma->virtualAddr;
    if (dma->virtualAddr)
    {
        return true;
    }

    return false;
}

static void Ba_DmaFree(BaDmaBuf* dma)
{
    if (dma)
    {
        free(dma->virtualAddr);
        memset(dma, 0, sizeof(BaDmaBuf));
    }
}

#ifdef __cplusplus
}
#endif
