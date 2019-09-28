#include "fh228/fh228_api.h"
#include "elog4c/elog4c.h"
#include "banana/ba_task.h"

#define LOG_TAG "case11"

const int channelCnt = 2;   /* 通道数 */

void case11()
{
    elog_i(LOG_TAG, "test NC to NT*n");
    elog_i(LOG_TAG, "usage:");
    elog_i(LOG_TAG, "    nt-> sp nt_case11,devId,speed,priority,suppressStatus,ncFcId,ntFcNum,myNtFcNum");
    elog_i(LOG_TAG, "    nc-> sp nc_case11,devId,speed,sendBytes,priority,suppressStatus,ncFcId,ntFcNum,sendCnt");
    elog_i(LOG_TAG, "please start NT first");
}

void nt_case11(
    const int devId,
    const int speed,
    const int priority,
    const int suppressStatus,
    const int ncFcId,
    const int ntFcNum,
    const int myNtFcNum) 
{
    int i = 0;
    int jumNtNum = 0;
    const int recvBytes = 64*1024 - 8;
	const int sendBytes = 0x1000000;
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));
    unsigned char *recvBuf = (unsigned char *)malloc(recvBytes);
    unsigned char *sendBuf = (unsigned char*)malloc(sendBytes);
	
    elog_d(LOG_TAG, "I am NT, my fcid = 0x%06x", (0x100000 + myNtFcNum));

	// 初始化发送数据的内容
    memset(sendBuf, 0, sendBytes);
    for (i = 0; i < sendBytes; ++i)
    {
        sendBuf[i] = (char)(i % 255 + 1);
    }
	
    // 初始化配置
    memset(fh228cfg, 0, sizeof(Fh228Config));
    fh228cfg->baseInfo.role                 = FH228_ROLE_NT;
    fh228cfg->baseInfo.fcid                 = 0x100000 + myNtFcNum;
    fh228cfg->baseInfo.creditNum            = 8;
    fh228cfg->baseInfo.speed                = speed;
    fh228cfg->baseInfo.ntTimeoutMicrosec    = 10000;
    fh228cfg->baseInfo.nt2ntTimeoutMicrosec = 10000;
    fh228cfg->baseInfo.edtov                = 10;
    fh228cfg->baseInfo.isRedundantEnabled   = 1;


    fh228cfg->channelCount = channelCnt;
    for (i = 0; i < channelCnt; i++)
    {
        fh228cfg->channels[i].subAddr        = myNtFcNum + jumNtNum;    /* 使下一个子地址跳跃NT节点总数的大小 */
        fh228cfg->channels[i].priority       = priority;
        fh228cfg->channels[i].sid            = fh228cfg->baseInfo.fcid;
        fh228cfg->channels[i].did            = ncFcId;
        fh228cfg->channels[i].otherDid       = 0x000000;
        fh228cfg->channels[i].otherSubAddr   = 0;
        fh228cfg->channels[i].suppressStatus = suppressStatus;
        fh228cfg->channels[i].retryNum       = 0;
        fh228cfg->channels[i].tr             = i % 2;
        fh228cfg->channels[i].dataByteCount  = recvBytes;
        jumNtNum += ntFcNum;
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

void nc_case11(
    const int devId,
    const int speed,
    const int sendBytes,
    const int priority,
    const int suppressStatus,
    const int ncFcId,
    const int ntFcNum,
    const int sendCnt)
{
    int i = 0;
    int j = 0;
    int x = 0;
    int ret = 0;
    int jumNtNum = 0;
    const int recvBytes = 16 * 1024 * 1024;
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));
    unsigned char *sendBuf = (unsigned char *)malloc(sendBytes);
	unsigned char *recvBuf = (unsigned char*)malloc(recvBytes);

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

    fh228cfg->channelCount = channelCnt * ntFcNum;

    for (j = 1; j <= ntFcNum; j++)
    {
        jumNtNum = 0;
        for (x = 0; x < channelCnt; x++)
        {
            fh228cfg->channels[i].subAddr          = j + jumNtNum;
            fh228cfg->channels[i].priority         = priority;
            fh228cfg->channels[i].sid              = fh228cfg->baseInfo.fcid;
            fh228cfg->channels[i].did              = 0x100000 + j;
            fh228cfg->channels[i].otherDid         = 0x000000;
            fh228cfg->channels[i].otherSubAddr     = 0;
            fh228cfg->channels[i].suppressStatus   = suppressStatus;
            fh228cfg->channels[i].retryNum         = 0;
            fh228cfg->channels[i].tr               = x % 2;
            fh228cfg->channels[i].dataByteCount    = sendBytes;
            ++i;
            jumNtNum += ntFcNum;
        }
        
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
			for (x = 0; x < fh228cfg->channelCount; x++)
			{
				unsigned int sa = fh228cfg->channels[x].subAddr;
				int len 		= fh228cfg->channels[x].dataByteCount;
	
				if (fh228cfg->channels[x].tr == FH228_DATA_DIRECT_TX)
				{
					// 发数据
					elog_v(LOG_TAG, "[sa=%d]:send data. len=%d", sa, len);
					ret = Fh228_Send(devId, sa, (char*)sendBuf, len);
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
					ret = Fh228_Send(devId, sa, NULL, 0);
					if (ret != 0)
					{
						elog_e(LOG_TAG, "send cmd failed, sa=%u", sa);
						goto Exit0;
					}
	Retry0:
					recvLen = Fh228_Recv(devId, sa, (char*)recvBuf, recvBytes);
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
