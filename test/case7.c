#include "fh228/fh228_api.h"
#include "elog4c/elog4c.h"
#include "banana/ba_task.h"

#define LOG_TAG "case7"

#define _mycountof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#define MB(x) ((x) * 1024 * 1024)
#define KB(x) ((x) * 1024)

typedef enum
{
    TEST_NC2NT,
    TEST_NT2NC,
} TestDataDirect;

typedef struct
{
    unsigned int    sa;
    unsigned int    did;
    int             priority;
    int             suppressStatus;
    int             len;
    TestDataDirect  direct;
} TestChnInfo;

static TestChnInfo g_testChnInfo[] = {
    { 1, 0x100001, 0, 0, 16777216, TEST_NC2NT },
    { 2, 0x100002, 0, 0, 16777216, TEST_NC2NT },
    { 3, 0x100003, 0, 0, 16777216, TEST_NC2NT },
    { 4, 0x100004, 0, 0, 16777216, TEST_NC2NT },
    { 5, 0x100005, 0, 0, 16777216, TEST_NC2NT },
    { 6, 0x100006, 0, 0, 16777216, TEST_NC2NT },
    
    { 7, 0x100001, 0, 0, 65528, TEST_NT2NC },
    { 8, 0x100002, 0, 0, 65528, TEST_NT2NC },
    { 9, 0x100003, 0, 0, 65528, TEST_NT2NC },
    { 10, 0x100004, 0, 0, 65528, TEST_NT2NC },
    { 11, 0x100005, 0, 0, 65528, TEST_NT2NC },
    { 12, 0x100006, 0, 0, 65528, TEST_NT2NC },
};

static void updateNcNtFcId(const int ntFcId1, const int ntFcId2, const int ntFcId3,const int ntFcId4, const int ntFcId5, const int ntFcId6)
{
    int i;
    int ntFcIdArr[6];
    ntFcIdArr[0] = ntFcId1;
    ntFcIdArr[1] = ntFcId2;
    ntFcIdArr[2] = ntFcId3;
    ntFcIdArr[3] = ntFcId4;
    ntFcIdArr[4] = ntFcId5;
    ntFcIdArr[5] = ntFcId6;
    
    for (i = 0; i < _mycountof(g_testChnInfo); i++)
    {
        g_testChnInfo[i].did = ntFcIdArr[i % 6];
    }
}

void case7()
{
    elog_i(LOG_TAG, "final case, test NC with NT*6");
    elog_i(LOG_TAG, "usage:");
    elog_i(LOG_TAG, "    nt-> sp nt_case7,devId,speed,ncFcId,ntFcId1,ntFcId2,ntFcId3,ntFcId4,ntFcId5,ntFcId6,myFcId");
    elog_i(LOG_TAG, "    nc-> sp nc_case7,devId,speed,ncFcId,ntFcId1,ntFcId2,ntFcId3,ntFcId4,ntFcId5,ntFcId6,sendCnt");
    elog_i(LOG_TAG, "please start NT first");
}

void nt_case7(
    //const int devId,
    const int speed,
    const int ncFcId,
    const int ntFcId1,
    const int ntFcId2,
    const int ntFcId3,
    const int ntFcId4,
    const int ntFcId5,
    const int ntFcId6,    
    const int myFcId)
{
	const int devId = 0;
    const int recvBytes = 16 * 1024 * 1024;
    const int sendBytes = 16 * 1024 * 1024;
    int i = 0;
    int chnIdx = 0;
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));
    char *recvBuf = (char*)malloc(recvBytes);
    char *sendBuf = (char*)malloc(sendBytes);

    elog_d(LOG_TAG, "I am NT, my ncFcId=0x%06x", ncFcId);
    elog_d(LOG_TAG, "I am NT, my ntFcId1=0x%06x", ntFcId1);
    elog_d(LOG_TAG, "I am NT, my ntFcId2=0x%06x", ntFcId2);
    elog_d(LOG_TAG, "I am NT, my ntFcId3=0x%06x", ntFcId3);
    elog_d(LOG_TAG, "I am NT, my ntFcId4=0x%06x", ntFcId4);
    elog_d(LOG_TAG, "I am NT, my ntFcId5=0x%06x", ntFcId5);
    elog_d(LOG_TAG, "I am NT, my ntFcId6=0x%06x", ntFcId6);
    elog_d(LOG_TAG, "I am NT, my myFcId=0x%06x", myFcId);

    updateNcNtFcId(ntFcId1, ntFcId2, ntFcId3,ntFcId4, ntFcId5, ntFcId6);

    // 初始化发送数据的内容
    memset(sendBuf, 0, sendBytes);
    for (i = 0; i < sendBytes; ++i)
    {
        sendBuf[i] = (char)(i % 255 + 1);
    }

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

    chnIdx = 0;
    for (i = 0; i < _mycountof(g_testChnInfo); i++)
    {
        if (g_testChnInfo[i].did == myFcId)
        {
            fh228cfg->channels[chnIdx].sid            = myFcId;
            fh228cfg->channels[chnIdx].did            = ncFcId;
            fh228cfg->channels[chnIdx].otherDid       = 0x000000;
            fh228cfg->channels[chnIdx].otherSubAddr   = 0;
            fh228cfg->channels[chnIdx].retryNum       = 0;
            fh228cfg->channels[chnIdx].subAddr        = g_testChnInfo[i].sa;
            fh228cfg->channels[chnIdx].priority       = g_testChnInfo[i].priority;
            fh228cfg->channels[chnIdx].suppressStatus = g_testChnInfo[i].suppressStatus;
            fh228cfg->channels[chnIdx].tr             = g_testChnInfo[i].direct == TEST_NT2NC ? 0 : 1;
            fh228cfg->channels[chnIdx].dataByteCount  = g_testChnInfo[i].len;

            elog_d("config", "channel[%d]:sa=%u,priority=%d,suppress=%d,tr=%d,dataBytes=%d",
                chnIdx,
                fh228cfg->channels[chnIdx].subAddr,
                fh228cfg->channels[chnIdx].priority,
                fh228cfg->channels[chnIdx].suppressStatus,
                fh228cfg->channels[chnIdx].tr,
                fh228cfg->channels[chnIdx].dataByteCount);

            chnIdx++;
        }
    }
    fh228cfg->channelCount = chnIdx;

    elog_d(LOG_TAG, "My channel count = %d", fh228cfg->channelCount);

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

    // 预配数据
    for (i = 0; i < fh228cfg->channelCount; i++)
    {
        if (fh228cfg->channels[i].tr == FH228_DATA_DIRECT_TX)
        {
            unsigned int sa = fh228cfg->channels[i].subAddr;
            int len         = fh228cfg->channels[i].dataByteCount;
            if (0 != Fh228_NtSetPreplanData(devId, sa, (const char*)sendBuf, len, 1))
            {
                elog_e(LOG_TAG, "nt set preplan data failed, sa=%u", sa);
                goto Exit0;
            }
            elog_d("preplan", "sa=%u,dataBytes=%d", sa, len);
        }
    }

    // 不停接收数据
    for (;;)
    {
        for (i = 0; i < fh228cfg->channelCount; i++)
        {
            unsigned int sa = fh228cfg->channels[i].subAddr;
            int realLen = Fh228_Recv(devId, sa, (char*)recvBuf, recvBytes);
            if (realLen < 0)
            {
                elog_e(LOG_TAG, "error: recv data.len=%d", realLen);
                break;
            }
        }
    }

Exit0:
    free(fh228cfg);
    free(recvBuf);
    free(sendBuf);

    if (0 != Fh228_Close(devId))
    {
        elog_e(LOG_TAG, "error: call Fh228CloseDev return error");
    }

    elog_d(LOG_TAG, "test case finished");
}

void nc_case7(
    const int devId,
    const int speed,
    const int ncFcId,
    const int ntFcId1,
    const int ntFcId2,
    const int ntFcId3,
    const int ntFcId4,
    const int ntFcId5,
    const int ntFcId6,     
    const int sendCnt)
{
    const int sendBytes = 16 * 1024 * 1024;
    const int recvBytes = 16 * 1024 * 1024;
    int i = 0;
    int x = 0;
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));
    char *sendBuf = (char *)malloc(sendBytes);
    char *recvBuf = (char*)malloc(recvBytes);

    elog_d(LOG_TAG, "I am NC, my fcid=0x%06x", ncFcId);
    updateNcNtFcId(ntFcId1, ntFcId2, ntFcId3,ntFcId4, ntFcId5, ntFcId6);

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

    fh228cfg->channelCount = _mycountof(g_testChnInfo);
    for (i = 0; i < fh228cfg->channelCount; i++)
    {
        fh228cfg->channels[i].sid            = fh228cfg->baseInfo.fcid;
        fh228cfg->channels[i].otherDid       = 0x000000;
        fh228cfg->channels[i].otherSubAddr   = 0;
        fh228cfg->channels[i].retryNum       = 0;
        fh228cfg->channels[i].subAddr        = g_testChnInfo[i].sa;
        fh228cfg->channels[i].priority       = g_testChnInfo[i].priority;
        fh228cfg->channels[i].did            = g_testChnInfo[i].did;
        fh228cfg->channels[i].suppressStatus = g_testChnInfo[i].suppressStatus;
        fh228cfg->channels[i].tr             = g_testChnInfo[i].direct == TEST_NC2NT ? 0 : 1;
        fh228cfg->channels[i].dataByteCount  = g_testChnInfo[i].len;

        elog_d("config", "channel[%d]:sa=%u,did=0x%08x,priority=%d,suppress=%d,tr=%d,dataBytes=%d",
            i,
            fh228cfg->channels[i].subAddr,
            fh228cfg->channels[i].did,
            fh228cfg->channels[i].priority,
            fh228cfg->channels[i].suppressStatus,
            fh228cfg->channels[i].tr,
            fh228cfg->channels[i].dataByteCount);
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

    // 初始化发送数据的内容
    memset(sendBuf, 0, sendBytes);
    for (i = 0; i < sendBytes; ++i)
    {
        sendBuf[i] = (char)(i % 255 + 1);
    }

    for (i = 0; i < sendCnt; i++)
    {
        for (x = 0; x < fh228cfg->channelCount; x++)
        {
            unsigned int sa = fh228cfg->channels[x].subAddr;
            int len         = fh228cfg->channels[x].dataByteCount;

            if (fh228cfg->channels[x].tr == FH228_DATA_DIRECT_TX)
            {
                // 发数据
                elog_v(LOG_TAG, "[sa=%d]:send data. len=%d", sa, len);
                int ret = Fh228_Send(devId, sa, sendBuf, len);
                if (ret < 0)
                {
                    elog_e(LOG_TAG, "send data failed, sa=%u", sa);
                    goto Exit0;
                }
            }
            else
            {
                // 发命令
                int isPrint = 0;
                int recvLen = 0;
                elog_v(LOG_TAG, "[sa=%d]:send cmd", sa);
                int ret = Fh228_Send(devId, sa, NULL, 0);
                if (ret != 0)
                {
                    elog_e(LOG_TAG, "send cmd failed, sa=%u", sa);
                    goto Exit0;
                }
Retry0:
                recvLen = Fh228_Recv(devId, sa, recvBuf, recvBytes);
                if (0 == recvLen)
                {
                    if (isPrint == 0)
                    {
                        elog_w(LOG_TAG, "[sa=%u]cmd send succeed, but no data, retry reading...", sa);
                        isPrint = 1;
                    }
                    Ba_TaskSleep(1);
                    goto Retry0;
                }

                if (recvLen != len)
                {
                    elog_e(LOG_TAG, "nc.sa[%u] send cmd but recv data error, recv.len=%d, is not [%d]",
                        sa, recvLen, len);
                    goto Exit0;
                }
            }
        }
    }

    elog_d(LOG_TAG, "send finished");

Exit0:
    free(fh228cfg);
    free(sendBuf);
    free(recvBuf);

    elog_d(LOG_TAG, "test case finished");
}
