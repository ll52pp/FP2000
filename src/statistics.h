#ifndef STATISTICS_H
#define STATISTICS_H

#include "common_header.h"
#include "send_que_mgr.h"

typedef struct
{
    uint64_t txBytesTotal; ///< 总的发送数据大小
    uint64_t txBytesMsgNum[FH228_MAX_CHANNEL_COUNT];
    uint64_t txBytesPriority[FH228_PRIORITY_COUNT];
    uint64_t txPackageTotal;
    uint64_t txPackageMsgNum[FH228_MAX_CHANNEL_COUNT];
    uint64_t txPackagePriority[FH228_PRIORITY_COUNT];

    uint64_t rxBytesTotal;
    uint64_t rxBytesMsgNum[FH228_MAX_CHANNEL_COUNT];
    uint64_t rxBytesPriority[FH228_PRIORITY_COUNT];
    uint64_t rxPackageTotal;
    uint64_t rxPackageMsgNum[FH228_MAX_CHANNEL_COUNT];
    uint64_t rxPackagePriority[FH228_PRIORITY_COUNT];

    uint64_t ncCmdCountTotal;
    uint64_t ncCmdCountMsgNum[FH228_MAX_CHANNEL_COUNT];
    uint64_t ncCmdCountPriority[FH228_PRIORITY_COUNT];
    uint64_t ncCmdBytesTotal;
    uint64_t ncCmdBytesMsgNum[FH228_MAX_CHANNEL_COUNT];
    uint64_t ncCmdBytesPriority[FH228_PRIORITY_COUNT];
} StatisticsCtx;

extern StatisticsCtx* g_statistics;

void Statistics_Init();
void Statistics_Uninit();
void Statistics_AddSendBytes(int msgNo, int priority, int sendBytes);
void Statistics_AddSendPackage(int msgNo, int priority, int packageNum);
void Statistics_AddRecvBytes(int msgNo, int priority, int recvBytes);
void Statistics_AddRecvPackage(int msgNo, int priority, int packageNum);
void Statistics_AddNcCmdCount(int msgNo, int priority, int count);
void Statistics_AddNcCmdBytes(int msgNo, int priority, int bytes);

// 统计信息输出接口, 供查看统计信息时调用
void Statistics_Print();
void Statistics_PrintEx(unsigned int beginChannelNo, unsigned int endChannelNo);

#endif
