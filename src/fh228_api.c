#include "common_header.h"
#include "utils.h"
#include "statistics.h"
#include "dev_context.h"
#include "reg.h"
#include "recv_que_mgr.h"
#include "send_que_mgr.h"
#include "config_mgr.h"
#include "write.h"
#include "read.h"
#include "period_msg.h"
#include "dev_mgr.h"
#include "dma_info.h"

#define LOG_TAG "api"

// 内部函数在此声明
static void _Fh228_RegBaseInfo(const FpgaRegBaseAddr* reg, const Fh228BaseInfo *baseInfo);
static void _Fh228_PrintUserCfg(const Fh228Config *config);
static bool _Fh228_InitCfgMgr(DevContext *dc, const Fh228Config *config);

int Fh228_Open(const int devId)
{
    DevContext *dc = NULL;
    if (!DevMgr_Init())
    {
        elog_e(LOG_TAG, "error in DevMgr_Init()");
        return -1;
    }

    dc = DevMgr_GetDevice(devId);
    if (NULL == dc)
    {
        elog_e(LOG_TAG, "DevMgr_GetDevice(%d) return NULL", devId);
        return -2;
    }

    Reg_SetPortEnab(dc->reg, 0);
    Reg_ResetFpga(dc->reg, 1);
    Reg_ClearIndirectRegister(dc->reg);

    Statistics_Init();
    SendQueMgr_Init(&dc->sendQueMgr, dc->reg);
    RecvQueMgr_Init(&dc->recvQueMgr, dc->reg);
    RecvTask_Init(&dc->recvTask, &dc->cfgCtx, &dc->recvQueMgr);
    DevCtx_SetDevOpened(dc, true);

    return 0;
}

int Fh228_Close(const int devId)
{
    DevContext *dc = DevMgr_GetDevice(devId);
    if (NULL == dc)
    {
        elog_e(LOG_TAG, "DevMgr_GetDevice(%d) return NULL", devId);
        return -1;
    }

    if (DevCtx_IsDevOpened(dc))
    {
        Reg_SetPortEnab(dc->reg, 0);
        RecvTask_Uninit(&dc->recvTask);
        Reg_ResetFpga(dc->reg, 1);
        Reg_ClearIndirectRegister(dc->reg);
        DevCtx_SetDevOpened(dc, false);

        SendQueMgr_Uninit(&dc->sendQueMgr);
        RecvQueMgr_Uninit(&dc->recvQueMgr);
    }

    DevMgr_Uninit();
    return 0;
}

int Fh228_SetConfig(const int devId, const Fh228Config *config)
{
    DevContext *dc = DevMgr_GetDevice(devId);
    if (NULL == dc)
    {
        elog_e(LOG_TAG, "DevMgr_GetDevice(%d) return NULL", devId);
        return -1;
    }

    _Fh228_RegBaseInfo(dc->reg, &config->baseInfo);

    if (false == _Fh228_InitCfgMgr(dc, config))
    {
        elog_e(LOG_TAG, "init config manager failed");
        return -2;
    }

    RecvTask_Start(&dc->recvTask); // 启动后台接收任务
    Reg_SetPortEnab(dc->reg, 1); // 激活FPGA端口
    _Fh228_PrintUserCfg(config); // 打印到控制台
    return 0;
}

int Fh228_Send(const int devId, const unsigned int subAddr, const char *buf, const int bytes)
{
    DevContext *dc = DevMgr_GetDevice(devId);
    if (NULL == dc)
    {
        elog_e(LOG_TAG, "DevMgr_GetDevice(%d) return NULL", devId);
        return -1;
    }
    return Write_SendDataOrCmd(dc, subAddr, buf, bytes);
}

int Fh228_Recv(const int devId, const unsigned int subAddr, char *buf, const int length)
{
    DevContext *dc = DevMgr_GetDevice(devId);
    if (NULL == dc)
    {
        elog_e(LOG_TAG, "DevMgr_GetDevice(%d) return NULL", devId);
        return -1;
    }
    return Read_FromChnQue(dc, subAddr, buf, length);
}

int Fh228_NtSetPreplanData(const int devId, const unsigned int subAddr, const char *buf, const int bytes, const int bForce)
{
    DevContext *dc = DevMgr_GetDevice(devId);
    if (NULL == dc)
    {
        elog_e(LOG_TAG, "devId=%d not found", devId);
        return -1;
    }

    if (!buf)
    {
        elog_e(LOG_TAG, "data point is null");
        return -1;
    }

    if (bytes > TX_NODE_SIZE_PREPLAN)
    {
        elog_e(LOG_TAG, "your buf.size=%d is too large, is must less than %d", bytes, TX_NODE_SIZE_PREPLAN);
        return -1;
    }

    if (false == CfgMgr_IsNtRole(&dc->cfgCtx))
    {
        elog_e(LOG_TAG, "sorry, you are not NT, so you cannot set perlan data");
        return -1;
    }

    if (false == Write_SetPreplanData(dc, subAddr, buf, bytes, bForce))
    {
        return -1;
    }

    return 0;
}

int Fh228_SetPeriodicMsgCmd(const int devId, Fh228PeriodicMsgCmd* cmds, const int count)
{
    DevContext *dc = DevMgr_GetDevice(devId);
    if (NULL == dc)
    {
        elog_e(LOG_TAG, "DevMgr_GetDevice(%d) return NULL", devId);
        return -1;
    }

    if (PeriodMsg_SetCmd(dc->reg, cmds, count))
    {
        return 0;
    }

    return -1;
}

static void _Fh228_RegBaseInfo(const FpgaRegBaseAddr* reg, const Fh228BaseInfo *baseInfo)
{
    // 配置一些基本的寄存器
    Reg_SetPortFcId(reg, baseInfo->fcid);
    Reg_SetPortCreditNum(reg, baseInfo->creditNum);
    Reg_SetPortSpeed(reg, baseInfo->speed);
    Reg_SetPortDidFilterEnab(reg, 1);
    Reg_SetPortEdtov(reg, baseInfo->edtov);
    Reg_SetPortRedundantEnab(reg, baseInfo->isRedundantEnabled);
    Reg_SetNtTimeOut(reg, baseInfo->ntTimeoutMicrosec);
    Reg_SetNt2NtTimeOut(reg, baseInfo->nt2ntTimeoutMicrosec);
    Reg_SetRatovTimeout(reg, 10000);
    Reg_SetRtcControlRtcBase(reg, baseInfo->speed == 1 ? 0 : 1);
}

static void _Fh228_PrintUserCfg(const Fh228Config *config)
{
    elog_v(LOG_TAG, "[userConfig] role                 = %s", config->baseInfo.role == FH228_ROLE_NC ? "NC" : "NT");
    elog_v(LOG_TAG, "[userConfig] fcid                 = 0x%06x", config->baseInfo.fcid);
    elog_v(LOG_TAG, "[userConfig] creditNum            = %d", config->baseInfo.creditNum);
    elog_v(LOG_TAG, "[userConfig] speed                = %d", config->baseInfo.speed);
    elog_v(LOG_TAG, "[userConfig] edtov                = %d", config->baseInfo.edtov);
    elog_v(LOG_TAG, "[userConfig] isRedundantEnabled   = %d", config->baseInfo.isRedundantEnabled);
    elog_v(LOG_TAG, "[userConfig] ntTimeoutMicrosec    = %d", config->baseInfo.ntTimeoutMicrosec);
    elog_v(LOG_TAG, "[userConfig] nt2ntTimeoutMicrosec = %d", config->baseInfo.nt2ntTimeoutMicrosec);
    elog_v(LOG_TAG, "[userConfig] channelCount         = %d", config->channelCount);
}

static bool _Fh228_InitCfgMgr(DevContext *dc, const Fh228Config *config)
{
    bool suc = true;
    SendQueContext* periodQue = SendQueMgr_GetPreplanMsgQue(&dc->sendQueMgr);
    CfgMgr_Init(&dc->cfgCtx, dc->reg);
    CfgMgr_SetRole(&dc->cfgCtx, config->baseInfo.role);
    CfgMgr_SetChnCfg(&dc->cfgCtx, config->channels, config->channelCount, periodQue);

    if (FH228_ROLE_NT == config->baseInfo.role)
    {
        suc = CfgMgr_SetConditionHashAndMap(&dc->cfgCtx);
    }

    if (!suc)
    {
        CfgMgr_Uninit(&dc->cfgCtx);
    }

    return suc;
}
