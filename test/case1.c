#include "fh228/fh228_api.h"
#include "elog4c/elog4c.h"
#include "banana/ba_task.h"

#define LOG_TAG "case1"

extern void Statistics_Print();
extern void Statistics_PrintEx(unsigned int beginChannelNo, unsigned int endChannelNo);

void x()
{
    Statistics_Print();
}

void y()
{
    Statistics_PrintEx(0, 20);
}

void showGod()
{
    printf("                      _ooOoo_                        \n");
    printf("                     o8888888o                       \n");
    printf("                     88\" . \"88                     \n");
    printf("                     (| -_- |)                       \n");
    printf("                     O\\  =  /O                      \n");
    printf("                  ____/`---'\\____                   \n");
    printf("                .'  \\\\|     |//  `.                \n");
    printf("               /  \\\\|||  :  |||//  \\              \n");
    printf("              /  _||||| -:- |||||-  \\               \n");
    printf("              |   | \\\\\\  -  /// |   |             \n");
    printf("              | \\_|  ''\\---/''  |   |              \n");
    printf("              \\  .-\\__  `-`  ___/-. /              \n");
    printf("            ___`. .'  /--.--\\  `. . __              \n");
    printf("         ."" '<  `.___\\_<|>_/___.'  >'"".           \n");
    printf("        | | :  `- \\`.;`\\ _ /`;.`/ - ` : | |        \n");
    printf("        \\  \\ `-.   \\_ __\\ /__ _/   .-` /  /      \n");
    printf("   ======`-.____`-.___\\_____/___.-`____.-'======    \n");
    printf("                      `=---='                        \n");
    printf("  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   \n");
    printf("        God bless FP2000, no bug, no crash           \n");
}

void case1()
{
    elog_i(LOG_TAG, "test NC -> NT");
    elog_i(LOG_TAG, "usage:");
    elog_i(LOG_TAG, "    nt-> sp nt_case1,devId,speed,channelCnt,recvCnt,recvBytes,priority,suppressStatus,ncFcId,ntFcId");
    elog_i(LOG_TAG, "    nc-> sp nc_case1,devId,speed,channelCnt,sendCnt,sendBytes,priority,suppressStatus,ncFcId,ntFcId");
    elog_i(LOG_TAG, "please start NT first");
}

void nt_case1(
    const int devId,
    const int speed,
    const int channelCnt,
    const int recvCnt,
    const int recvBytes,
    const int priority,
    const int suppressStatus,
    const int ncFcId,
    const int ntFcId)
{
    int i = 0;
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));
    unsigned char *recvBuf = (unsigned char *)malloc(recvBytes);

    elog_i(LOG_TAG, "I am NT, my fcid=0x%06x, my did=0x%06x", ntFcId, ncFcId);

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
        fh228cfg->channels[i].subAddr        = i + 1;
        fh228cfg->channels[i].priority       = priority;
        fh228cfg->channels[i].sid            = fh228cfg->baseInfo.fcid;
        fh228cfg->channels[i].did            = ncFcId;
        fh228cfg->channels[i].otherDid       = 0x000000;
        fh228cfg->channels[i].otherSubAddr   = 0;
        fh228cfg->channels[i].suppressStatus = suppressStatus;
        fh228cfg->channels[i].retryNum       = 0;
        fh228cfg->channels[i].tr             = FH228_DATA_DIRECT_RX;
        fh228cfg->channels[i].dataByteCount  = recvBytes;
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
        for (i = 0; i < channelCnt; i++)
        {
            unsigned int sa = i + 1;
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

    if (0 != Fh228_Close(devId))
    {
        elog_e(LOG_TAG, "error: call Fh228CloseDev return error");
    }

    elog_d(LOG_TAG, "test case finished");
}

void nc_case1(
    const int devId,
    const int speed,
    const int channelCnt,
    const int sendCnt,
    const int sendBytes,
    const int priority,
    const int suppressStatus,
    const int ncFcId,
    const int ntFcId)
{
    int i = 0;
    int x = 0;
    unsigned int ret = 0;
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));
    unsigned char *sendBuf = (unsigned char *)malloc(sendBytes);

    elog_i(LOG_TAG, "I am NC, my fcid=0x%06x, my did=0x%06x", ncFcId, ntFcId);

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
        fh228cfg->channels[i].tr             = FH228_DATA_DIRECT_TX;
        fh228cfg->channels[i].dataByteCount  = sendBytes;
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
        sendBuf[i] = (unsigned char)(i % 255 + 1);
    }

    for (i = 0; i < sendCnt; i++)
    {
        for (x = 0; x < channelCnt; x++)
        {
            unsigned int sa = x + 1;
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
