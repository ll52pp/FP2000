#pragma once
#include "ba_base_types.h"
#include <assert.h>
#include <wdm.h>
#include <wdftypes.h>
#include <wdfobject.h>
#include <wdfDmaEnabler.h>
#include "wdfCommonBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

// 请在外面提供一个全局函数, 用于获取当前设备对象句柄
extern WDFDEVICE GetCurrentWdfDevice();

typedef struct
{
    void*           virtualAddr;
    LARGE_INTEGER   logicalAddr;
    // ------------------------------------------------
    WDFDMAENABLER   _enabler; ///< 内部属性,请不要在你的代码里直接使用这个变量
    int             _alignOffset; ///< 基于数据对齐做的偏移
} BaDmaBuf;

static void Ba_DmaFree(BaDmaBuf* dma)
{
    if (dma)
    {
        if (dma->_enabler)
        {
            WdfObjectDelete(dma->_enabler);
        }
        memset(dma, 0, sizeof(BaDmaBuf));
    }
}

static bool Ba_DmaAlloc(const size_t size, const size_t alignBytes, BaDmaBuf* dma)
{
    bool suc = false;
    NTSTATUS status;
    WDF_OBJECT_ATTRIBUTES attributes;
    WDF_DMA_ENABLER_CONFIG dmaConfig;
    WDFCOMMONBUFFER commonBuffer;

    assert(0 != size);
    assert(NULL != dma);

    memset(dma, 0, sizeof(BaDmaBuf));

    WDF_DMA_ENABLER_CONFIG_INIT(&dmaConfig, WdfDmaProfilePacket, size);
    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.ParentObject = NULL;
    status = WdfDmaEnablerCreate(GetCurrentWdfDevice(), &dmaConfig, &attributes, &dma->_enabler);
    if (!NT_SUCCESS(status))
    {
        goto Exit0;
    }

    status = WdfCommonBufferCreate(dma->_enabler, size, WDF_NO_OBJECT_ATTRIBUTES, &commonBuffer);
    if (!NT_SUCCESS(status))
    {
        goto Exit0;
    }

    dma->logicalAddr = WdfCommonBufferGetAlignedLogicalAddress(commonBuffer);
    dma->virtualAddr = WdfCommonBufferGetAlignedVirtualAddress(commonBuffer);

    if (dma->logicalAddr.QuadPart && dma->virtualAddr)
    {
        suc = true;
    }

Exit0:
    if (!suc)
    {
        Ba_DmaFree(dma);
    }

    return suc;
}

#ifdef __cplusplus
}
#endif
