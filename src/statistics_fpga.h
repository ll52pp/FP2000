#ifndef STATISTICS_FPGA_H
#define STATISTICS_FPGA_H

#include "common_header.h"
#include "fpga_reg_addr.h"

typedef struct
{
    uint64_t recvFrameNumTotal;
    uint64_t recvIUNumTotal;
    uint64_t recvErrorIUNumTotal;
    uint64_t recvDiscardFrameNumTotal;
    uint64_t recvInvalidFrameNumTotal;
    uint64_t sendFrameNumTotal;
    uint64_t sendIUNumTotal;
} StatisticsMacStatInfo;

typedef struct
{
    uint64_t RxUndersizeFrameCnt;
    uint64_t RxOversizeFrameErrCnt;
    uint64_t RxAllFrameCnt;
    uint64_t RxValidFrameCnt;
    uint64_t RxWordCnt;
    uint64_t RxRRDYCnt;
    uint64_t RxKErrCnt;
    uint64_t LinkChgUp2DownCnt;
    uint64_t TxAllFrameCnt;
    uint64_t TxValidFrameCnt;
    uint64_t TxWordCnt;
    uint64_t TxRRDYCnt;
    uint64_t TxKErrCnt;
} StatisticsPortInfo;

void StatisticsFpga_Get(const FpgaRegBaseAddr* reg, StatisticsMacStatInfo* macInfo, StatisticsPortInfo* portInfo);

#endif
