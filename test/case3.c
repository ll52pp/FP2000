#include "fh228/fh228_api.h"
#include "elog4c/elog4c.h"
#include "banana/ba_task.h"

#define LOG_TAG "case3"

#define TEST_NC_SEND_BYTES 16777216
#define TEST_NT_SEND_BYTES (64 * 1024 - 8)

void case3()
{
    elog_i(LOG_TAG, "test mix NT and NC, NC send=16MB, NT send=64KB-8");
    elog_i(LOG_TAG, "usage:");
    elog_i(LOG_TAG, "    nt-> sp nt_case3,devId,speed,priority,suppressStatus,channelCnt,ncFcId,ntFcId");
    elog_i(LOG_TAG, "    nc-> sp nc_case3,devId,speed,priority,suppressStatus,channelCnt,ncFcId,ntFcId,testCnt");
    elog_i(LOG_TAG, "please start NT first");
}

void nt_case3(
    const int devId,
    const int speed,
    const int priority,
    const int suppressStatus,
    const int channelCnt,
    const int ncFcId,
    const int ntFcId)
{
    const int idxSend = 0;
    const int idxRecv = 1;
    const int dataBytes[2] = { TEST_NT_SEND_BYTES, TEST_NC_SEND_BYTES };
    const int sendBytes = dataBytes[idxSend];
    const int recvBytes = dataBytes[idxRecv];

    char *sendBuf = (char*)malloc(sendBytes);
    char *recvBuf = (char*)malloc(recvBytes);
    int i = 0;
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));

    // 初始化发送数据的内容
    memset(sendBuf, 0, sendBytes);
    for (i = 0; i < sendBytes; ++i)
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
        fh228cfg->channels[i].otherDid       = 0x000000;
        fh228cfg->channels[i].otherSubAddr   = 0;
        fh228cfg->channels[i].suppressStatus = suppressStatus;
        fh228cfg->channels[i].retryNum       = 0;
        fh228cfg->channels[i].tr             = i % 2; // 收发交叉在一起
        fh228cfg->channels[i].dataByteCount  = dataBytes[fh228cfg->channels[i].tr];
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
        unsigned int sa = fh228cfg->channels[i].subAddr;
        if (FH228_DATA_DIRECT_RX == fh228cfg->channels[i].tr)
        {
            continue;
        }

        if (0 != Fh228_NtSetPreplanData(devId, sa, (const char*)sendBuf, sendBytes, 1))
        {
            elog_e(LOG_TAG, "nt set preplan data failed, sa=%u", sa);
            goto Exit0;
        }
    }
    elog_d(LOG_TAG, "preplan data config finished");

    for (;;)
    {
        for (i = 0; i < fh228cfg->channelCount; i++)
        {
            unsigned int sa = fh228cfg->channels[i].subAddr;
            int realLen = Fh228_Recv(devId, sa, (char*)recvBuf, recvBytes);
            if (realLen < 0)
            {
                elog_e(LOG_TAG, "error: recv data.len=%d, sa=%u", realLen, sa);
                break;
            }
        }
    }

Exit0:
    free(fh228cfg);
    free(sendBuf);
    free(recvBuf);
}

void nc_case3(
    const int devId,
    const int speed,
    const int priority,
    const int suppressStatus,
    const int channelCnt,
    const int ncFcId,
    const int ntFcId,
    const int testCnt)
{
    const int idxSend = 0;
    const int idxRecv = 1;
    const int dataBytes[2] = { TEST_NC_SEND_BYTES, TEST_NT_SEND_BYTES };
    const int sendBytes = dataBytes[idxSend];
    const int recvBytes = dataBytes[idxRecv];

    char *sendBuf = (char*)malloc(sendBytes);
    char *recvBuf = (char*)malloc(recvBytes);

    int i = 0;
    int x = 0;
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));

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
        fh228cfg->channels[i].tr             = !(i % 2); // 正好跟NT反着来收发交叉在一起
        fh228cfg->channels[i].dataByteCount  = dataBytes[fh228cfg->channels[i].tr];
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
        for (i = 0; i < fh228cfg->channelCount; i++)
        {
            int realLen = -1;
            unsigned int sa = fh228cfg->channels[i].subAddr;
            if (FH228_DATA_DIRECT_RX == fh228cfg->channels[i].tr) // 发命令
            {
                if (0 != Fh228_Send(devId, sa, NULL, 0))
                {
                    elog_e(LOG_TAG, "nc.sa[%u] send cmd failed", sa);
                    goto Exit0;
                }

Retry0:
                realLen = Fh228_Recv(devId, sa, (char*)recvBuf, recvBytes);
                if (0 == realLen)
                {
                    elog_w(LOG_TAG, "[sa=%u]cmd send succed, but no data, retry reading...", sa);
                    Ba_TaskSleep(1);
                    goto Retry0;
                }

                if (realLen != recvBytes)
                {
                    elog_e(LOG_TAG, "nc.sa[%u] send cmd but recv data error, recv.len=%d, is not [%d]",
                        sa, realLen, recvBytes);
                    goto Exit0;
                }
            }
            else // 发数据
            {
                if (sendBytes != Fh228_Send(devId, sa, sendBuf, sendBytes))
                {
                    elog_e(LOG_TAG, "nc.sa[%u] send data failed", sa);
                    goto Exit0;
                }
            }
        }
    }

Exit0:
    free(fh228cfg);
    free(recvBuf);
    elog_d(LOG_TAG, "tese case finished");
}
