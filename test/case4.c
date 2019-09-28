#include "fh228/fh228_api.h"
#include "elog4c/elog4c.h"
#include "banana/ba_task.h"

#define LOG_TAG "case4"

void case4()
{
    elog_i(LOG_TAG, "test NC to NT*2");
    elog_i(LOG_TAG, "usage:");
    elog_i(LOG_TAG, "    nt-> sp nt_case4,devId,speed,recvBytes,priority,suppressStatus,ncFcId,myFcId");
    elog_i(LOG_TAG, "    nc-> sp nc_case4,devId,speed,sendBytes,priority,suppressStatus,ncFcId,ntFcId1,ntFcId2,sendCnt");
    elog_i(LOG_TAG, "please start NT first");
}

void nt_case4(
    const int devId,
    const int speed,
    const int recvBytes,
    const int priority,
    const int suppressStatus,
    const int ncFcId,
    const int myFcId)
{
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));
    unsigned char *recvBuf = (unsigned char *)malloc(recvBytes);

    elog_d(LOG_TAG, "I am NT, my fcid = 0x%06x", myFcId);

    // 初始化配置
    memset(fh228cfg, 0, sizeof(Fh228Config));
    fh228cfg->baseInfo.role                 = FH228_ROLE_NT;
    fh228cfg->baseInfo.fcid                 = myFcId;
    fh228cfg->baseInfo.creditNum            = 8;
    fh228cfg->baseInfo.speed                = speed;
    fh228cfg->baseInfo.ntTimeoutMicrosec    = 10000;
    fh228cfg->baseInfo.nt2ntTimeoutMicrosec = 10000;
    fh228cfg->baseInfo.edtov                = 10;
    fh228cfg->baseInfo.isRedundantEnabled   = 1;

    fh228cfg->channelCount                  = 1;
    fh228cfg->channels[0].sid               = myFcId;
    fh228cfg->channels[0].did               = ncFcId;
    fh228cfg->channels[0].otherDid          = 0x000000;
    fh228cfg->channels[0].otherSubAddr      = 0;
    fh228cfg->channels[0].retryNum          = 0;
    fh228cfg->channels[0].tr                = FH228_DATA_DIRECT_RX;
    fh228cfg->channels[0].subAddr           = myFcId * 256;
    fh228cfg->channels[0].priority          = priority;
    fh228cfg->channels[0].suppressStatus    = suppressStatus;
    fh228cfg->channels[0].dataByteCount     = recvBytes;

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

    for (;;)
    {
        unsigned int sa = fh228cfg->channels[0].subAddr;
        int realLen = Fh228_Recv(devId, sa, (char*)recvBuf, recvBytes);
        if (realLen < 0)
        {
            elog_e(LOG_TAG, "error: recv data.len=%d", realLen);
            break;
        }
    }

Exit0:
    free(fh228cfg);
    free(recvBuf);

    if (0 != Fh228_Close(devId))
    {
        elog_e(LOG_TAG, "error: call Fh228CloseDev return error");
    }

    elog_d(LOG_TAG, "test case finished");
}

void nc_case4(
    const int devId,
    const int speed,
    const int sendBytes,
    const int priority,
    const int suppressStatus,
    const int ncFcId,
    const int ntFcId1,
    const int ntFcId2,
    const int sendCnt)
{
    int i = 0;
    int x = 0;
    unsigned int ret = 0;
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));
    unsigned char *sendBuf = (unsigned char *)malloc(sendBytes);

    // 初始化配置
    memset(fh228cfg, 0, sizeof(Fh228Config));
    fh228cfg->baseInfo.role                 = FH228_ROLE_NC;
    fh228cfg->baseInfo.fcid                 = ncFcId;
    fh228cfg->baseInfo.creditNum            = 8;
    fh228cfg->baseInfo.speed                = speed;
    fh228cfg->baseInfo.ntTimeoutMicrosec    = 10000;
    fh228cfg->baseInfo.nt2ntTimeoutMicrosec = 10000;
    fh228cfg->baseInfo.edtov                = 10;
    fh228cfg->baseInfo.isRedundantEnabled   = 1;

    fh228cfg->channelCount = 2;
    fh228cfg->channels[0].subAddr        = ntFcId1 * 256;
    fh228cfg->channels[0].did            = ntFcId1;
    fh228cfg->channels[0].priority       = priority;
    fh228cfg->channels[0].sid            = fh228cfg->baseInfo.fcid;
    fh228cfg->channels[0].otherDid       = 0x000000;
    fh228cfg->channels[0].otherSubAddr   = 0;
    fh228cfg->channels[0].suppressStatus = suppressStatus;
    fh228cfg->channels[0].retryNum       = 0;
    fh228cfg->channels[0].tr             = FH228_DATA_DIRECT_TX;
    fh228cfg->channels[0].dataByteCount  = sendBytes;

    fh228cfg->channels[1].subAddr        = ntFcId2 * 256;
    fh228cfg->channels[1].did            = ntFcId2;
    fh228cfg->channels[1].priority       = priority;
    fh228cfg->channels[1].sid            = fh228cfg->baseInfo.fcid;
    fh228cfg->channels[1].otherDid       = 0x000000;
    fh228cfg->channels[1].otherSubAddr   = 0;
    fh228cfg->channels[1].suppressStatus = suppressStatus;
    fh228cfg->channels[1].retryNum       = 0;
    fh228cfg->channels[1].tr             = FH228_DATA_DIRECT_TX;
    fh228cfg->channels[1].dataByteCount  = sendBytes;

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

    // 初始化发送数据的内容
    memset(sendBuf, 0, sendBytes);
    for (i = 0; i < sendBytes; ++i)
    {
        sendBuf[i] = (unsigned char)(i % 255 + 1);
    }

    for (i = 0; i < sendCnt; i++)
    {
        for (x = 0; x < 2; x++)
        {
            unsigned int sa = fh228cfg->channels[x].subAddr;
            ret = Fh228_Send(devId, sa, (const char*)sendBuf, sendBytes);
            if (ret <= 0)
            {
                Ba_TaskSleep(1);
            }
        }
    }

    elog_d(LOG_TAG, "send finished");

Exit0:
    free(fh228cfg);
    free(sendBuf);

    elog_d(LOG_TAG, "test case finished");
}
