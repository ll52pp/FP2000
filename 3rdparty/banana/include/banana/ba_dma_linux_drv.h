#ifndef BA_DMA_LINUX_DRV_H_7BDEB968_0DC9_4349_B801_D46A96446195
#define BA_DMA_LINUX_DRV_H_7BDEB968_0DC9_4349_B801_D46A96446195

#include <linux/dma-mapping.h>
#include "ba_base_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// 请在外面提供一个全局函数, 用于获取当前设备对象句柄
extern struct device* GetCurrentDevice(void);

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
    void*           _rawVirtualAddr;
    dma_addr_t      _dmaHandle;
    size_t          _size;
} BaDmaBuf;

static bool Ba_DmaAlloc(const size_t size, const size_t alignBytes, BaDmaBuf* dma)
{
    memset(dma, 0, sizeof(BaDmaBuf));
    dma->_rawVirtualAddr = (char*)dma_alloc_coherent(GetCurrentDevice(), size + alignBytes, &dma->_dmaHandle, GFP_KERNEL);
    if (dma->_rawVirtualAddr)
    {
        dma->_size = size;
        dma->virtualAddr = dma->_rawVirtualAddr;
        memcpy(&dma->logicalAddr, &dma->_dmaHandle, sizeof(LARGE_INTEGER));
        return true;
    }
    return false;
}

static void Ba_DmaFree(BaDmaBuf* dma)
{
    if (dma)
    {
        dma_free_coherent(GetCurrentDevice(), dma->_size, dma->_rawVirtualAddr, dma->_dmaHandle);
        memset(dma, 0, sizeof(BaDmaBuf));
    }
}

#ifdef __cplusplus
}
#endif

#endif
