#include "statistics_fpga.h"
#include "utils.h"

#define LOG_TAG "statistics_fpga"

/*读统计寄存器使用的偏移*/
#define MAC_STAT_RX_FRAME_NUM           0x1
#define MAC_STAT_RX_IU_NUM              0x2
#define MAC_STAT_RX_ERROR_IU_NUM        0x3
#define MAC_STAT_RX_DISCARD_IU_NUM      0x4
#define MAC_STAT_RX_INVALID_FRAME_NUM   0x5
#define MAC_STAT_TX_FRAME_NUM           0xB
#define MAC_STAT_TX_IU_NUM              0xC
#define MAC_STAT_P0_BASE                0x30
#define MAC_STAT_P1_BASE                0x10
#define PORT_STAT_RX_UNDERSIZE_FRAME    0x0
#define PORT_STAT_RX_OVERSIZE_FRAME     0x1
#define PORT_STAT_RX_ALL_FRAME 		    0x2
#define PORT_STAT_RX_VALID_FRAME        0x3
#define PORT_STAT_RX_FRAME_LEN          0x4
#define PORT_STAT_LNK_CHG_UP2DWN        0x5
#define PORT_STAT_RX_RRDY               0x6
#define PORT_STAT_RX_K_ERR              0x7
#define PORT_STAT_TX_ALL_FRAME          0x8
#define PORT_STAT_TX_VALID_FRAME        0x9
#define PORT_STAT_TX_FRAME_LEN          0xA
#define PORT_STAT_TX_RRDY               0xB

typedef struct
{
#ifndef __BIG_ENDIAN__
    ULONG type : 6;  /* [ 0: 3] RW 0-cur timestamp 32'b~41'b
                     1-cur timestamp 0'b~31'b
                     2-recv word num
                     3-recv idle num
                     4-recv frame num
                     5-recv word num
                     6-recv idle num
                     7-recv frame num*/
    ULONG reserve : 25; /* [ 4:30] RW*/
    ULONG start : 1;  /* [   31] RW set 1 start statistic, self clear*/
#else
    ULONG start : 1;  /* [   31] RW set 1 start statistic, self clear*/
    ULONG reserve : 25; /* [ 4:30] RW*/
    ULONG type : 6;  /* [ 0: 3] RW 0-cur timestamp 32'b~41'b
                     1-cur timestamp 0'b~31'b
                     2-recv word num
                     3-recv idle num
                     4-recv frame num
                     5-recv word num
                     6-recv idle num
                     7-recv frame num*/
#endif
} StatisticsCtrl;

static UINT32 _GetByType(const FpgaRegBaseAddr* reg_hw, const UINT32 type)
{
    StatisticsCtrl ctrl;
    ZeroStruct(ctrl);
    ctrl.start = 0;
    ctrl.type = type;

    Ba_TaskSleep(10);
    Ba_RegisterWrite32Struct(&reg_hw->statisticsCtrl, ctrl);
    Ba_TaskSleep(10);
    return Ba_RegisterRead32(&reg_hw->statisticsData);
}

void StatisticsFpga_Get(const FpgaRegBaseAddr* reg, StatisticsMacStatInfo* macInfo, StatisticsPortInfo* portInfo)
{
    INT32 idx, port_base_addr;
    UINT64 t;
    StatisticsCtrl ctrl;
    ZeroStruct(ctrl);
    /*设置快照*/
    ctrl.start = 1;
    Ba_RegisterWrite32Struct(&reg->statisticsCtrl, ctrl);
    /*注意：快照设置后必须等待，立刻读会读到错误值*/
    Ba_TaskSleep(10);

    /*从FPGA取得统计数据*/
    t = _GetByType(reg, MAC_STAT_RX_FRAME_NUM);
    macInfo->recvFrameNumTotal += t;
    t = _GetByType(reg, MAC_STAT_RX_IU_NUM);
    macInfo->recvIUNumTotal += t;
    t = _GetByType(reg, MAC_STAT_RX_ERROR_IU_NUM);
    macInfo->recvErrorIUNumTotal += t;
    t = _GetByType(reg, MAC_STAT_RX_DISCARD_IU_NUM);
    macInfo->recvDiscardFrameNumTotal += t;
    t = _GetByType(reg, MAC_STAT_RX_INVALID_FRAME_NUM);
    macInfo->recvInvalidFrameNumTotal += t;
    t = _GetByType(reg, MAC_STAT_TX_FRAME_NUM);
    macInfo->sendFrameNumTotal += t;
    t = _GetByType(reg, MAC_STAT_TX_IU_NUM);
    macInfo->sendIUNumTotal += t;
    for (idx = 0; idx < 2; ++idx)
    {
        if (idx == 0)
        {
            port_base_addr = MAC_STAT_P0_BASE;
        }
        else
        {
            port_base_addr = MAC_STAT_P1_BASE;
        }
        t = _GetByType(reg, port_base_addr + PORT_STAT_RX_UNDERSIZE_FRAME);
        portInfo[idx].RxUndersizeFrameCnt += t;
        t = _GetByType(reg, port_base_addr + PORT_STAT_RX_OVERSIZE_FRAME);
        portInfo[idx].RxOversizeFrameErrCnt += t;
        t = _GetByType(reg, port_base_addr + PORT_STAT_RX_ALL_FRAME);
        portInfo[idx].RxAllFrameCnt += t;
        t = _GetByType(reg, port_base_addr + PORT_STAT_RX_VALID_FRAME);
        portInfo[idx].RxValidFrameCnt += t;
        t = _GetByType(reg, port_base_addr + PORT_STAT_RX_FRAME_LEN);
        portInfo[idx].RxWordCnt += t;
        t = _GetByType(reg, port_base_addr + PORT_STAT_RX_RRDY);
        portInfo[idx].RxRRDYCnt += t;
        t = _GetByType(reg, port_base_addr + PORT_STAT_RX_K_ERR);
        portInfo[idx].RxKErrCnt += t;
        t = _GetByType(reg, port_base_addr + PORT_STAT_LNK_CHG_UP2DWN);
        portInfo[idx].LinkChgUp2DownCnt += t;
        t = _GetByType(reg, port_base_addr + PORT_STAT_TX_ALL_FRAME);
        portInfo[idx].TxAllFrameCnt += t;
        t = _GetByType(reg, port_base_addr + PORT_STAT_TX_VALID_FRAME);
        portInfo[idx].TxValidFrameCnt += t;
        t = _GetByType(reg, port_base_addr + PORT_STAT_TX_FRAME_LEN);
        portInfo[idx].TxWordCnt += t;
        t = _GetByType(reg, port_base_addr + PORT_STAT_TX_RRDY);
        portInfo[idx].TxRRDYCnt += t;
    }
}
