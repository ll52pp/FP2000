#ifndef DEV_CONTEXT_H
#define DEV_CONTEXT_H

#include "common_header.h"
#include "struct.h"
#include "send_que_mgr.h"
#include "recv_que_mgr.h"
#include "config_mgr.h"
#include "recv_task.h"

/**
 * @brief �豸������
 *  ��¼��һ��FPGA�豸����ʱ����Ҫ��������Դ��Ϣ
 */
typedef struct
{
    bool                isDevOpened; ///< �豸�Ƿ��ڴ�״̬
    int                 funcNo;
    int                 deviceNo;
    int                 busNo;
    uint8_t             irq;
    CfgMgrCtx           cfgCtx;      ///< ������Ϣ
    RecvTaskCtx         recvTask;    ///< ��������
    FpgaRegBaseAddr*    reg;         ///< �Ĵ�������ַ
    SendQueMgrContext   sendQueMgr;  ///< ���ն��й�����
    RecvQueMgrContext   recvQueMgr;  ///< ���Ͷ��й�����
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
