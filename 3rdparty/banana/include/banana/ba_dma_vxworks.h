#ifndef BA_DMA_VXWORKS_H_7BDEB968_0DC9_4349_B801_D46A96446195
#define BA_DMA_VXWORKS_H_7BDEB968_0DC9_4349_B801_D46A96446195
/**************************************************************
 * 1.封装Vxworks底层接口
 * 2.
 * ************************************************************/
#include "ba_base_types.h"
#include <cacheLib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DUMMYSTRUCTNAME
typedef union _LARGE_INTEGER {
    struct {
        ULONG LowPart;
        long HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        ULONG LowPart;
        long HighPart;
    } u;
    UINT64 QuadPart;
} LARGE_INTEGER;

typedef struct
{
    void*           virtualAddr;
    LARGE_INTEGER   logicalAddr;
    // ------------------------------------------------
    void*           _rawAddr;
} BaDmaBuf;

static size_t _Ba_CalcByteAlignment(const size_t size, const size_t align)
{
    return ((size + align - 1) & (~(align - 1)));
}

static bool Ba_DmaAlloc(const size_t size, const size_t alignBytes, BaDmaBuf* dma)
{
    assert(0 != size);
    assert(NULL != dma);

    memset(dma, 0, sizeof(BaDmaBuf));
    dma->_rawAddr = (char*)cacheDmaMalloc(size + alignBytes);
    if (dma->_rawAddr)
    {
        dma->virtualAddr = (void*)_Ba_CalcByteAlignment((UINT32)dma->_rawAddr, alignBytes);
        dma->logicalAddr.LowPart = (UINT32)dma->virtualAddr;
        return true;
    }
    return false;
}

static void Ba_DmaFree(BaDmaBuf* dma)
{
    if (dma)
    {
        cacheDmaFree(dma->_rawAddr);
        memset(dma, 0, sizeof(BaDmaBuf));
    }
}

#ifdef __cplusplus
}
#endif

#endif
