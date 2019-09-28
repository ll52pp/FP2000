#include "statistics.h"

#define LOG_TAG "statistics"

StatisticsCtx* g_statistics = NULL;

static StatisticsCtx* Statistics_GetInstance()
{
    if (!g_statistics)
    {
        g_statistics = malloc(sizeof(StatisticsCtx));
        memset(g_statistics, 0, sizeof(StatisticsCtx));
    }
    return g_statistics;
}

void Statistics_Init()
{
    Statistics_Uninit();
}

void Statistics_Uninit()
{
    StatisticsCtx* ctx = Statistics_GetInstance();
    memset(ctx, 0, sizeof(StatisticsCtx));
}

void Statistics_Print()
{
    int i = 0;
    StatisticsCtx* ctx = Statistics_GetInstance();

    elog_i(LOG_TAG, "statistics: total: ---------");
    // 收发包计数
    elog_i(LOG_TAG, "txPackage.total = %llu", ctx->txPackageTotal);
    elog_i(LOG_TAG, "rxPackage.total = %llu", ctx->rxPackageTotal);
    // 收发字节数
    elog_i(LOG_TAG, "txBytes.total = %llu", ctx->txBytesTotal);
    elog_i(LOG_TAG, "rxBytes.total = %llu", ctx->rxBytesTotal);
    // 命令统计
    elog_i(LOG_TAG, "cmdCount.total = %llu", ctx->ncCmdCountTotal);
    elog_i(LOG_TAG, "cmdBytes.total = %llu", ctx->ncCmdBytesTotal);

    elog_i(LOG_TAG, "statistics: priority: ------");
    // 优先级收发包计数
    for (i = 0; i < FH228_PRIORITY_COUNT; i++)
    {
        if (ctx->txPackagePriority[i] > 0)
        {
            elog_i(LOG_TAG, "txPackage.priority[%d] = %llu", i, ctx->txPackagePriority[i]);
        }
        if (ctx->rxPackagePriority[i] > 0)
        {
            elog_i(LOG_TAG, "rxPackage.priority[%d] = %llu", i, ctx->rxPackagePriority[i]);
        }
    }
    // 优先级收发包字节数
    for (i = 0; i < FH228_PRIORITY_COUNT; i++)
    {
        if (ctx->txBytesPriority[i] > 0)
        {
            elog_i(LOG_TAG, "txBytes.priority[%d] = %llu", i, ctx->txBytesPriority[i]);
        }
        if (ctx->rxBytesPriority[i] > 0)
        {
            elog_i(LOG_TAG, "rxBytes.priority[%d] = %llu", i, ctx->rxBytesPriority[i]);
        }
    }
    // 优先级发送命令统计
    for (i = 0; i < FH228_PRIORITY_COUNT; i++)
    {
        if (ctx->ncCmdCountPriority[i] > 0)
        {
            elog_i(LOG_TAG, "cmdCount.priority[%d] = %llu", i, ctx->ncCmdCountPriority[i]);
        }
    }
    for (i = 0; i < FH228_PRIORITY_COUNT; i++)
    {
        if (ctx->ncCmdBytesPriority[i] > 0)
        {
            elog_i(LOG_TAG, "cmdBytes.priority[%d] = %llu", i, ctx->ncCmdBytesPriority[i]);
        }
    }
}

void Statistics_PrintEx(unsigned int beginChannelNo, unsigned int endChannelNo)
{
    unsigned int i = 0;
    StatisticsCtx* ctx = Statistics_GetInstance();

    if (beginChannelNo >= FH228_MAX_CHANNEL_COUNT)
    {
        beginChannelNo = FH228_MAX_CHANNEL_COUNT - 1;
    }

    if (endChannelNo >= FH228_MAX_CHANNEL_COUNT)
    {
        endChannelNo = FH228_MAX_CHANNEL_COUNT - 1;
    }

    elog_i(LOG_TAG, "statistics: detail: --------");
    // 各通道字节包数统计
    for (i = beginChannelNo; i <= endChannelNo; i++)
    {
        if (ctx->txPackageMsgNum[i] > 0)
        {
            elog_i(LOG_TAG, "txPackage.msgNo[%d] = %llu", i, ctx->txPackageMsgNum[i]);
        }
    }
    for (i = beginChannelNo; i <= endChannelNo; i++)
    {
        if (ctx->rxPackageMsgNum[i] > 0)
        {
            elog_i(LOG_TAG, "rxPackage.msgNo[%d] = %llu", i, ctx->rxPackageMsgNum[i]);
        }
    }
    // 各通道字节数统计
    for (i = beginChannelNo; i <= endChannelNo; i++)
    {
        if (ctx->txBytesMsgNum[i] > 0)
        {
            elog_i(LOG_TAG, "txBytes.msgNo[%d] = %llu", i, ctx->txBytesMsgNum[i]);
        }
    }
    for (i = beginChannelNo; i <= endChannelNo; i++)
    {
        if (ctx->rxBytesMsgNum[i] > 0)
        {
            elog_i(LOG_TAG, "rxBytes.msgNo[%d] = %llu", i, ctx->rxBytesMsgNum[i]);
        }
    }
    // 各通道命令数统计
    for (i = beginChannelNo; i <= endChannelNo; i++)
    {
        if (ctx->ncCmdCountMsgNum[i] > 0)
        {
            elog_i(LOG_TAG, "ncCmdCount.msgNo[%d] = %llu", i, ctx->ncCmdCountMsgNum[i]);
        }
    }
}

void Statistics_AddSendBytes(int msgNo, int priority, int sendBytes)
{
    StatisticsCtx* ctx = Statistics_GetInstance();
    ctx->txBytesTotal += sendBytes;
    ctx->txBytesMsgNum[msgNo] += sendBytes;
    ctx->txBytesPriority[priority] += sendBytes;
}

void Statistics_AddSendPackage(int msgNo, int priority, int packageNum)
{
    StatisticsCtx* ctx = Statistics_GetInstance();
    ctx->txPackageTotal += packageNum;
    ctx->txPackageMsgNum[msgNo] += packageNum;
    ctx->txPackagePriority[priority] += packageNum;
}

void Statistics_AddRecvBytes(int msgNo, int priority, int recvBytes)
{
    StatisticsCtx* ctx = Statistics_GetInstance();
    ctx->rxBytesTotal += recvBytes;
    ctx->rxBytesMsgNum[msgNo] += recvBytes;
    ctx->rxBytesPriority[priority] += recvBytes;
}

void Statistics_AddRecvPackage(int msgNo, int priority, int packageNum)
{
    StatisticsCtx* ctx = Statistics_GetInstance();
    ctx->rxPackageTotal += packageNum;
    ctx->rxPackageMsgNum[msgNo] += packageNum;
    ctx->rxPackagePriority[priority] += packageNum;
}

void Statistics_AddNcCmdCount(int msgNo, int priority, int count)
{
    StatisticsCtx* ctx = Statistics_GetInstance();
    ctx->ncCmdCountTotal += count;
    ctx->ncCmdCountMsgNum[msgNo] += count;
    ctx->ncCmdCountPriority[priority] += count;
}

void (int msgNo, int priority, int bytes)
{
    StatisticsCtx* ctx = Statistics_GetInstance();
    ctx->ncCmdBytesTotal += bytes;
    ctx->ncCmdBytesMsgNum[msgNo] += bytes;
    ctx->ncCmdBytesPriority[priority] += bytes;
}
