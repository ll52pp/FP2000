#include "fh228/fh228_api.h"
#include "elog4c/elog4c.h"
#include "banana/ba_task.h"

#define LOG_TAG "case2"

void case2()
{
    elog_i(LOG_TAG, "test NT -> NC");
    elog_i(LOG_TAG, "usage:");
    elog_i(LOG_TAG, "    nt-> sp nt_case2,devId,speed,channelCnt,dataBytes,priority,suppressStatus,ncFcId,ntFcId");
    elog_i(LOG_TAG, "    nc-> sp nc_case2,devId,speed,channelCnt,dataBytes,priority,suppressStatus,ncFcId,ntFcId,testCnt");
    elog_i(LOG_TAG, "please start NT first");
}

void nt_case2(
    const int devId,
    const int speed,
    const int channelCnt,
    const int dataBytes,
    const int priority,
    const int suppressStatus,
    const int ncFcId,
    const int ntFcId)
{
    char *sendBuf = (char*)malloc(dataBytes);
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));
    int i = 0;

    elog_i(LOG_TAG, "I am NT, my fcid=0x%06x, my did=0x%06x", ntFcId, ncFcId);

    // 初始化发送数据的内容
    memset(sendBuf, 0, dataBytes);
    for (i = 0; i < dataBytes; ++i)
    {
        sendBuf[i] = (char)(i % 255 + 1);
    }

    // 基本信息配置
    memset(fh228cfg, 0, sizeof(Fh228Config));
    fh228cfg->baseInfo.role                 = FH228_ROLE_NT;
    fh228cfg->baseInfo.fcid                 = ntFcId;
    fh228cfg->baseInfo.creditNum            = 8;
    fh228cfg->baseInfo.speed                = speed;
    fh228cfg->baseInfo.ntTimeoutMicrosec    = 10000;
    fh228cfg->baseInfo.nt2ntTimeoutMicrosec = 10000;
    fh228cfg->baseInfo.edtov                = 10;
    fh228cfg->baseInfo.isRedundantEnabled   = 1;

    // 配置消息通道
    fh228cfg->channelCount = channelCnt;
    for (i = 0; i < fh228cfg->channelCount; i++)
    {
        fh228cfg->channels[i].subAddr        = i + 1;
        fh228cfg->channels[i].priority       = priority;
        fh228cfg->channels[i].sid            = fh228cfg->baseInfo.fcid;
        fh228cfg->channels[i].did            = ncFcId;
        fh228cfg->channels[i].otherDid       = 0x100001;
        fh228cfg->channels[i].otherSubAddr   = 0;
        fh228cfg->channels[i].suppressStatus = suppressStatus;
        fh228cfg->channels[i].retryNum       = 0;
        fh228cfg->channels[i].tr             = FH228_DATA_DIRECT_TX;
        fh228cfg->channels[i].dataByteCount  = dataBytes;
    }

    if (0 != Fh228_Open(devId))
    {
        elog_e(LOG_TAG, "error: call Fh228OpenDev return error");
        goto Exit0;
    }

    if (0 != Fh228_SetConfig(devId, fh228cfg))
    {
        elog_e(LOG_TAG, "error: call Fh228SetConfig return error");
        goto Exit0;
    }

    for (i = 0; i < fh228cfg->channelCount; i++)
    {
        unsigned int sa = i + 1;
        if (0 != Fh228_NtSetPreplanData(devId, sa, (const char*)sendBuf, dataBytes, 1))
        {
            elog_e(LOG_TAG, "nt set preplan data failed, sa=%u", sa);
            goto Exit0;
        }
    }
    elog_d(LOG_TAG, "preplan data config finished");

Exit0:
    free(fh228cfg);
    free(sendBuf);
}

void nc_case2(
    const int devId,
    const int speed,
    const int channelCnt,
    const int dataBytes,
    const int priority,
    const int suppressStatus,
    const int ncFcId,
    const int ntFcId,
    const int testCnt)
{
    int i = 0;
    int x = 0;
    char *recvBuf = (char*)malloc(dataBytes);
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));

    elog_i(LOG_TAG, "I am NC, my fcid=0x%06x, my did=0x%06x", ncFcId, ntFcId);

    // 初始化配置
    memset(fh228cfg, 0, sizeof(Fh228Config));
    fh228cfg->baseInfo.role = FH228_ROLE_NC;
    fh228cfg->baseInfo.fcid                 = ncFcId;
    fh228cfg->baseInfo.creditNum            = 8;
    fh228cfg->baseInfo.speed                = speed;
    fh228cfg->baseInfo.ntTimeoutMicrosec    = 10000;
    fh228cfg->baseInfo.nt2ntTimeoutMicrosec = 10000;
    fh228cfg->baseInfo.edtov                = 10000;
    fh228cfg->baseInfo.isRedundantEnabled   = 1;

    fh228cfg->channelCount = channelCnt;
    for (i = 0; i < fh228cfg->channelCount; i++)
    {
        fh228cfg->channels[i].subAddr        = i + 1;
        fh228cfg->channels[i].priority       = priority;
        fh228cfg->channels[i].sid            = fh228cfg->baseInfo.fcid;
        fh228cfg->channels[i].did            = ntFcId;
        fh228cfg->channels[i].otherDid       = 0x000000;
        fh228cfg->channels[i].otherSubAddr   = 0;
        fh228cfg->channels[i].suppressStatus = suppressStatus;
        fh228cfg->channels[i].retryNum       = 0;
        fh228cfg->channels[i].tr             = FH228_DATA_DIRECT_RX;
        fh228cfg->channels[i].dataByteCount  = dataBytes;
    }

    if (0 != Fh228_Open(devId))
    {
        elog_e(LOG_TAG, "error: call Fh228OpenDev return error");
        goto Exit0;
    }

    if (0 != Fh228_SetConfig(devId, fh228cfg))
    {
        elog_e(LOG_TAG, "error: call Fh228SetConfig return error");
        goto Exit0;
    }

    for (x = 0; x < testCnt; x++)
    {
        int retryCnt = 0;
        for (i = 0; i < fh228cfg->channelCount; i++)
        {
            unsigned int sa = i + 1;
            int realLen = -1;

            if (0 != Fh228_Send(devId, sa, NULL, 0))
            {
                elog_e(LOG_TAG, "nc.sa[%u] send cmd failed", sa);
                goto Exit0;
            }

ReRecv:
            realLen = Fh228_Recv(devId, sa, (char*)recvBuf, dataBytes);
            if (0 == realLen)
            {
                if (retryCnt++ > 1000)
                {
                    elog_e(LOG_TAG, "can not recv data from sa=%u", sa);
                    goto Exit0;
                }
                Ba_TaskSleep(1);
                goto ReRecv;
            }

            if (realLen != dataBytes)
            {
                elog_e(LOG_TAG, "nc.sa[%u] recv data error, recv len=%d, is not %d", sa, realLen, dataBytes);
            }
        }
    }

Exit0:
    free(fh228cfg);
    free(recvBuf);

    elog_d(LOG_TAG, "tese case finished");
}
