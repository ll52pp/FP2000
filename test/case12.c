#include "fh228/fh228_api.h"
#include "elog4c/elog4c.h"
#include "banana/ba_task.h"

#define LOG_TAG "case12"

void case12()
{
    elog_i(LOG_TAG, "test NC -> NT");
    elog_i(LOG_TAG, "usage:");
    elog_i(LOG_TAG, "    nc-> sp nc_case12,devId,speed,channelCnt,dataBytes,priority,suppressStatus,ncFcId,ntFcId");
    elog_i(LOG_TAG, "    nt-> sp nc_case12,devId,speed,channelCnt,priority,suppressStatus,ncFcId,ntFcId");
    elog_i(LOG_TAG, "please start NT first");
}
#if 0
void nc_case12(
    const int devId,
    const int speed,
    const int channelCnt,
    const int dataBytes,
    const int priority,
    const int suppressStatus,
    const int ncFcId,
    const int ntFcId)
{
    const int cmdCount = channelCnt;    //周期消息指令个数
    char *sendBuf = (char*)malloc(dataBytes);
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));
    // +1是因为周期消息指令后有一个跳转指令
    Fh228PeriodicMsgCmd* cmds = (Fh228PeriodicMsgCmd*)malloc(sizeof(Fh228PeriodicMsgCmd) * (cmdCount + 1));
    int i = 0;

    elog_i(LOG_TAG, "I am NC, my fcid=0x%06x, my did=0x%06x", ncFcId, ntFcId);

    // 初始化发送数据的内容
    memset(sendBuf, 0, dataBytes);
    for (i = 0; i < dataBytes; ++i)
    {
        sendBuf[i] = (char)(i % 255 + 1);
    }
    // 初始化周期消息指令
    for (i = 0; i < cmdCount; ++i)
    {
        cmds[i].isEnabled    = 1;
        cmds[i].type         = 0;
        cmds[i].content      = 0;
        cmds[i].jumpIndex    = i + 1;
        cmds[i].timeInterval = 2;
    }
    // 初始化跳转指令
    cmds[cmdCount].isEnabled    = 1;
    cmds[cmdCount].type         = 0;
    cmds[cmdCount].content      = 7;
    cmds[cmdCount].jumpIndex    = 0;
    cmds[cmdCount].timeInterval = 2;

    // 基本信息配置
    memset(fh228cfg, 0, sizeof(Fh228Config));
    fh228cfg->baseInfo.role                 = FH228_ROLE_NC;
    fh228cfg->baseInfo.fcid                 = ncFcId;
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
        fh228cfg->channels[i].did            = ntFcId;
        fh228cfg->channels[i].otherDid       = 0x000000;
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
        if (0 != Fh228_NcSetPreplanData(devId, sa, (const char*)sendBuf, dataBytes, 1))
        {
            elog_e(LOG_TAG, "nc set PeriodicMsg failed, sa=%u", sa);
            goto Exit0;
        }
    }

    // 配置周期消息指令
    if (0 != Fh228_SetPeriodicMsgCmd(devId, cmds, cmdCount))
    {
        elog_e(LOG_TAG, "error: call Fh228_SetPeriodicMsgCmd return error");
        goto Exit0;
    }

    elog_d(LOG_TAG, "PeriodicMsg config finished");

Exit0:
    free(fh228cfg);
    free(sendBuf);
    free(cmds);
}
#endif

void nt_case12(
    const int devId,
    const int speed,
    const int channelCnt,
    const int priority,
    const int suppressStatus,
    const int ncFcId,
    const int ntFcId)
{
    const int recvBytes = 0x1000000;
    int i = 0;
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));
    unsigned char *recvBuf = (unsigned char *)malloc(recvBytes);

    elog_d(LOG_TAG, "I am NT, my fcid = 0x%06x", ntFcId);

    // 初始化配置
    memset(fh228cfg, 0, sizeof(Fh228Config));
    fh228cfg->baseInfo.role                 = FH228_ROLE_NT;
    fh228cfg->baseInfo.fcid                 = ntFcId;
    fh228cfg->baseInfo.creditNum            = 8;
    fh228cfg->baseInfo.speed                = speed;
    fh228cfg->baseInfo.ntTimeoutMicrosec    = 10000;
    fh228cfg->baseInfo.nt2ntTimeoutMicrosec = 10000;
    fh228cfg->baseInfo.edtov                = 10;
    fh228cfg->baseInfo.isRedundantEnabled   = 1;

    fh228cfg->channelCount = channelCnt;
    for (i = 0; i < fh228cfg->channelCount; i++)
    {
        fh228cfg->channels[i].did               = ncFcId;
        fh228cfg->channels[i].otherDid          = 0x000000;
        fh228cfg->channels[i].otherSubAddr      = 0;
        fh228cfg->channels[i].retryNum          = 0;
        fh228cfg->channels[i].tr                = FH228_DATA_DIRECT_RX;
        fh228cfg->channels[i].subAddr           = i + 1;
        fh228cfg->channels[i].priority          = priority;
        fh228cfg->channels[i].suppressStatus    = suppressStatus;
        fh228cfg->channels[i].dataByteCount     = recvBytes;

        elog_d("chnCfg", "did=0x%06x,sa=%d",
            fh228cfg->channels[i].did,
            fh228cfg->channels[i].subAddr
        );
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

    for (;;)
    {
        for (i = 0; i < fh228cfg->channelCount; i++)
        {
            unsigned int sa = i + 1;
            int realLen = Fh228_Recv(devId, sa, (char*)recvBuf, recvBytes);
            if (realLen < 0)
            {
                elog_e(LOG_TAG, "error: recv data.len=%d, sa=%d", realLen, sa);
                break;
            }
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


