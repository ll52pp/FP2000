#include "dev_context.h"

void DevCtx_Init(DevContext* ctx)
{
    memset(ctx, 0, sizeof(DevContext));
    ctx->isDevOpened = false;
    ctx->busNo = -1;
    ctx->deviceNo = -1;
    ctx->funcNo = -1;
    ctx->reg = NULL;
    ctx->irq = 0xff;
}

void DevCtx_SetBusNo(DevContext* ctx, int busNo)
{
    ctx->busNo = busNo;
}

void DevCtx_SetDevNo(DevContext* ctx, int devNo)
{
    ctx->deviceNo = devNo;
}

void DevCtx_SetFuncNo(DevContext* ctx, int funcNo)
{
    ctx->funcNo = funcNo;
}

void DevCtx_SetRegBaseAddr(DevContext* ctx, FpgaRegBaseAddr* reg)
{
    ctx->reg = reg;
}

void DevCtx_SetIrq(DevContext* ctx, uint8_t irq)
{
    ctx->irq = irq;
}

void DevCtx_SetDevOpened(DevContext* ctx, const bool isOpened)
{
    ctx->isDevOpened = isOpened;
}

bool DevCtx_IsDevOpened(DevContext* ctx)
{
    return ctx->isDevOpened;
}
