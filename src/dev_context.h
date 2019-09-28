#ifndef DEV_CONTEXT_H
#define DEV_CONTEXT_H

#include "common_header.h"
#include "struct.h"
#include "send_que_mgr.h"
#include "recv_que_mgr.h"
#include "config_mgr.h"
#include "recv_task.h"

/**
 * @brief 设备上下文
 *  记录了一个FPGA设备运行时所需要的所有资源信息
 */
typedef struct
{
    bool                isDevOpened; ///< 设备是否处于打开状态
    int                 funcNo;
    int                 deviceNo;
    int                 busNo;
    uint8_t             irq;
    CfgMgrCtx           cfgCtx;      ///< 配置信息
    RecvTaskCtx         recvTask;    ///< 接收任务
    FpgaRegBaseAddr*    reg;         ///< 寄存器基地址
    SendQueMgrContext   sendQueMgr;  ///< 接收队列管理器
    RecvQueMgrContext   recvQueMgr;  ///< 发送队列管理器
} DevContext;

void DevCtx_Init(DevContext* ctx);
void DevCtx_SetBusNo(DevContext* ctx, int busNo);
void DevCtx_SetDevNo(DevContext* ctx, int devNo);
void DevCtx_SetFuncNo(DevContext* ctx, int funcNo);
void DevCtx_SetRegBaseAddr(DevContext* ctx, FpgaRegBaseAddr* reg);
void DevCtx_SetIrq(DevContext* ctx, uint8_t irq);
void DevCtx_SetDevOpened(DevContext* ctx, const bool isOpened);
bool DevCtx_IsDevOpened(DevContext* ctx);

#endif
