#include "fh228/fh228_api.h"
#include "elog4c/elog4c.h"
#include "banana/ba_task.h"
#include <sys/time.h>


#define LOG_TAG "case"

const int ncFcId = 0x100000;
const int devId    = 0;     //�豸��
const int speed    = 2;     //���� 
const int priority = 0;
const int suppressStatus = 0;

extern void Statistics_Print();
void nt(const int myNtNo);
void nt2nc(const int myNtNo, Fh228Config* fh228cfg, const int dataBytes, int start, int end);
void nt2nt_snt(const int myNtNo, Fh228Config* fh228cfg, const int dataBytes, int start, int end);
void recvNt(const int myNtNo, Fh228Config* fh228cfg, const int dataBytes);
void SetConfig(Fh228Config* fh228cfg, const int myNtNo, const int ntBaseAddr);

/*************************
*1�Žڵ�ʹ�õ��ӵ�ַ���£�
*NT2NT:1/2/3/4/5 ���ڷ��ͣ������ӵ�ַ���ڽ���
*NC2NT:31~40 ���ڽ���, ����1024
*NC2NT:41~50 ���ڽ��գ�����64k
*NC2NT:51~60 ���ڽ��գ�����16M
*NT2NC:61~70 ���ڷ��ͣ�����1024
*NT2NC:71~80 ���ڷ��ͣ�����64k


*2�Žڵ�ʹ�õ��ӵ�ַ���£�
*NT2NT:6/7/8/9/10 ���ڷ��ͣ������ӵ�ַ���ڽ���
*NC2NT:31~40 ���ڽ���, ����1024
*NC2NT:41~50 ���ڽ��գ�����64k
*NC2NT:51~60 ���ڽ��գ�����16M
*NT2NC:61~70 ���ڷ��ͣ�����1024
*NT2NC:71~80 ���ڷ��ͣ�����64k

*3�Žڵ�ʹ�õ��ӵ�ַ���£�
*NT2NT:11/12/13/14/15 ���ڷ��ͣ������ӵ�ַ���ڽ���
*NC2NT:31~40 ���ڽ���, ����1024
*NC2NT:41~50 ���ڽ��գ�����64k
*NC2NT:51~60 ���ڽ��գ�����16M
*NT2NC:61~70 ���ڷ��ͣ�����1024
*NT2NC:71~80 ���ڷ��ͣ�����64k

*4�Žڵ�ʹ�õ��ӵ�ַ���£�
*NT2NT:16/17/18/19/20 ���ڷ��ͣ������ӵ�ַ���ڽ���
*NC2NT:31~40 ���ڽ���, ����1024
*NC2NT:41~50 ���ڽ��գ�����64k
*NC2NT:51~60 ���ڽ��գ�����16M
*NT2NC:61~70 ���ڷ��ͣ�����1024
*NT2NC:71~80 ���ڷ��ͣ�����64k

*5�Žڵ�ʹ�õ��ӵ�ַ���£�
*NT2NT:21/22/23/24/25 ���ڷ��ͣ������ӵ�ַ���ڽ���
*NC2NT:31~40 ���ڽ���, ����1024
*NC2NT:41~50 ���ڽ��գ�����64k
*NC2NT:51~60 ���ڽ��գ�����16M
*NT2NC:61~70 ���ڷ��ͣ�����1024
*NT2NC:71~80 ���ڷ��ͣ�����64k

*6�Žڵ�ʹ�õ��ӵ�ַ���£�
*NT2NT:26/27/28/29/30 ���ڷ��ͣ������ӵ�ַ���ڽ���
*NC2NT:31~40 ���ڽ���, ����1024
*NC2NT:41~50 ���ڽ��գ�����64k
*NC2NT:51~60 ���ڽ��գ�����16M
*NT2NC:61~70 ���ڷ��ͣ�����1024
*NT2NC:71~80 ���ڷ��ͣ�����64k
**************************/

int case_main(const int myNtNo)
{
    
    if (0 != Fh228_Open(devId))
    {
        elog_e(LOG_TAG, "error: call Fh228OpenDev return error");
        return -1;
    }
    
    nt(myNtNo);

    
    elog_d(LOG_TAG, "error, can't go there!");
    
    if (0 != Fh228_Close(devId))
    {
        elog_e(LOG_TAG, "error: call Fh228CloseDev return error");
    }
    return 0;
}

void nt(const int myNtNo)
{
    int ntBaseAddr = 0;
    
    Fh228Config* fh228cfg = (Fh228Config*)malloc(sizeof(Fh228Config));
    elog_i(LOG_TAG, "I am NT%d, my fcid=0x%06x", myNtNo, 0x100000 + myNtNo);

    switch (myNtNo)
    {
        case 1:
           ntBaseAddr = 0;
           break;
        case 2:
           ntBaseAddr = 5;
           break;
        case 3:
           ntBaseAddr = 10;
           break;
        case 4:
           ntBaseAddr = 15;
           break;
        case 5:
           ntBaseAddr = 20;
           break;
        case 6:
           ntBaseAddr = 25;
           break;
    }

    // ��ʼ������
    SetConfig(fh228cfg, myNtNo, ntBaseAddr);

    if (0 != Fh228_SetConfig(devId, fh228cfg))
    {
        elog_e(LOG_TAG, "error: call Fh228SetConfig return error");
        goto Exit0;
    }

    //nt2nt_snt(myNtNo, fh228cfg, 64*1024, 0, 5);          //��������������ʾ���ڵ�����������nt2nt_snt��ͨ��λ��
    nt2nt_snt(myNtNo, fh228cfg, 1024, 0, 5);               //��������������ʾ���ڵ�����������nt2nt_snt��ͨ��λ��8
    nt2nt_snt(myNtNo, fh228cfg, 1024, 102, 107);           //��������������ʾ���ڵ�����������nt2nt_snt��ͨ��λ��8
    nt2nc(myNtNo, fh228cfg, 1024, 60, 70);                 //��������������ʾ���ڵ�����������nt2nc��ͨ��λ��
    nt2nc(myNtNo, fh228cfg, 64*1024, 70, 80);              //��������������ʾ���ڵ�����������nt2nc��ͨ��λ��
    nt2nc(myNtNo, fh228cfg, 1024, 96, 102);                //��������������ʾ���ڵ�����������nt2nc��ͨ��λ��
    recvNt(myNtNo, fh228cfg, 16*1024*1024);                //nt2nt_rnt��nc2nt�Ľ��ն�
    
Exit0:
    free(fh228cfg);
}




void nt2nc(const int myNtNo, Fh228Config* fh228cfg, const int dataBytes, int start, int end)
{
    int i = 0;

    // ��ʼ���������ݵ�����
    char *sendBuf = (char*)malloc(dataBytes);
    memset(sendBuf, 0, dataBytes);
    for (i = 0; i < dataBytes; ++i)
    {
        sendBuf[i] = (char)(i % 255 + 1);
    }

    for (i = start; i < end; i++)
    {
        unsigned int sa = fh228cfg->channels[i].subAddr;
        if (0 != Fh228_NtSetPreplanData(devId, sa, (const char*)sendBuf, dataBytes, 1))
        {
            elog_e(LOG_TAG, "nt set preplan data failed, sa=%u", sa);
            goto Exit0;
        }
    }
    elog_d(LOG_TAG, "set nt2nc finished");

Exit0:
    free(sendBuf);
}


void nt2nt_snt(const int myNtNo, Fh228Config* fh228cfg, const int dataBytes, int start, int end)
{
    int i    = 0;
    
    // ��ʼ���������ݵ�����
    char *sendBuf = (char*)malloc(dataBytes);
    memset(sendBuf, 0, dataBytes);
    for (i = 0; i < dataBytes; ++i)
    {
        sendBuf[i] = (char)(i % 255 + 1);
    }

    for (i = 0; i < 5; i++)
    {
        unsigned int sa = fh228cfg->channels[i].subAddr;
        if (0 != Fh228_NtSetPreplanData(devId, sa, (const char*)sendBuf, dataBytes, 1))
        {
            elog_e(LOG_TAG, "nt set preplan data failed, sa=%u", sa);
            goto Exit0;
        }
    }
    elog_d(LOG_TAG, "preplan data config finished");
    
Exit0:
    free(sendBuf);
}

/*��nt2nt_rnt��nc2nt�еĽ��ղ��ַŵ�һ�����recvNt�������Ͳ��ô�����������*/
void recvNt(const int myNtNo, Fh228Config* fh228cfg, const int dataBytes)
{
    int i = 0;
    int flag = 0;

    int start1 = 5;
    int end1   = 30;
    int start2 = 30;
    int end2   = 60;
    int start3 = 90;
    int end3   = 96;
    int start4 = 102;
    int end4   = 107;

    
    int start = start1;
    int end = end1;
    long long t0 = 0;
    long long t1 = 0;
    
            
    unsigned char *recvBuf = (unsigned char *)malloc(dataBytes);
    
    elog_i(LOG_TAG, "=======info: start recv=======");

    for (;;)
    {
        for (i = start; i < end; i++)
        {
            unsigned int sa = fh228cfg->channels[i].subAddr;
            int realLen = Fh228_Recv(devId, sa, (char*)recvBuf, dataBytes);
            if (realLen < 0)
            {
                elog_e(LOG_TAG, "error: recv data.len=%d", realLen);
                break;
            }
            if (realLen > 0){
                t0 = tickGet();
            }
           
        }

        t1 = tickGet();
        if ((t1 - t0) >= sysClkRateGet() && t0 != 0)
        {
           Statistics_Print();
           elog_d(LOG_TAG, "=================================================");
           elog_d(LOG_TAG, " ");
           elog_d(LOG_TAG, " ");
           elog_d(LOG_TAG, " ");
           elog_d(LOG_TAG, " ");
           t0 = 0;
        }        
        
        if(flag == 1)
        {
            start = start2;
            end = end2;
            flag = 2;
        }
        else if (flag == 2)
        {
            start = start3;
            end = end3;
            flag = 3;
        }
        else if (flag == 3)
        {
            start = start4;
            end = end4;
            flag = 0;
        }
        else if (flag == 0)
        {
            start = start1;
            end = end1;
            flag = 1;
        }
    }
    
    free(recvBuf);
}


void SetConfig(Fh228Config* fh228cfg, const int myNtNo, const int ntBaseAddr)
{
    int i = 0;
    int did  = 0;   //Ŀ�Ľڵ��ID
    int subAddr = 0;
    memset(fh228cfg, 0, sizeof(Fh228Config));
    fh228cfg->baseInfo.role                 = FH228_ROLE_NT;
    fh228cfg->baseInfo.fcid                 = 0x100000 + myNtNo;
    fh228cfg->baseInfo.creditNum            = 8;
    fh228cfg->baseInfo.speed                = speed;
    fh228cfg->baseInfo.ntTimeoutMicrosec    = 10000;
    fh228cfg->baseInfo.nt2ntTimeoutMicrosec = 10000;
    fh228cfg->baseInfo.edtov                = 10;
    fh228cfg->baseInfo.isRedundantEnabled   = 0;

    fh228cfg->channelCount = 97;    
    //1.NT->NT ���Ͷˣ����÷�����Ϣͨ��(��5��ͨ��)
    did = 0x100001;    //��did����ʼֵ����1�Žڵ��fcid
    for (i = 0; i < 5; i++)
    {
        fh228cfg->channels[i].subAddr        = ntBaseAddr + i + 1;
        fh228cfg->channels[i].priority       = priority;
        fh228cfg->channels[i].sid            = fh228cfg->baseInfo.fcid;
        
        if (did == fh228cfg->baseInfo.fcid) //���ͷ���did���������Լ���id
        {
            did++;
        }
        fh228cfg->channels[i].did = did;
        did++;
        fh228cfg->channels[i].nt2nt          = 1;
        fh228cfg->channels[i].otherDid       = 0x000000;
        fh228cfg->channels[i].otherSubAddr   = 0;
        fh228cfg->channels[i].suppressStatus = suppressStatus;
        fh228cfg->channels[i].retryNum       = 0;
        fh228cfg->channels[i].tr             = FH228_DATA_DIRECT_TX;
        //fh228cfg->channels[i].dataByteCount  = 64*1024;
        fh228cfg->channels[i].dataByteCount  = 1024;
    }
    
    //2.NT->NT ���նˣ����ý�����Ϣͨ��(��25��ͨ��)
    subAddr = 1;    
    for (i = 5; i < 30; i++)
    {
        //���ӵ�ַ���ڷ����ӵ�ַʱ������
        while(subAddr == ntBaseAddr + 1 ||
              subAddr == ntBaseAddr + 2 ||
              subAddr == ntBaseAddr + 3 ||
              subAddr == ntBaseAddr + 4 ||
              subAddr == ntBaseAddr + 5)
        {
            subAddr++;
        }
        fh228cfg->channels[i].subAddr        = subAddr;
        subAddr++;
        fh228cfg->channels[i].priority       = priority;
        fh228cfg->channels[i].sid            = fh228cfg->baseInfo.fcid;
        fh228cfg->channels[i].did            = ncFcId;
        fh228cfg->channels[i].otherDid       = 0x000000;
        fh228cfg->channels[i].otherSubAddr   = 0;
        fh228cfg->channels[i].suppressStatus = suppressStatus;
        fh228cfg->channels[i].retryNum       = 0;
        fh228cfg->channels[i].nt2nt          = 0;
        fh228cfg->channels[i].tr             = FH228_DATA_DIRECT_RX;
        //fh228cfg->channels[i].dataByteCount  = 64*1024;
        fh228cfg->channels[i].dataByteCount  = 1024;
    }

    //3.NC->NT ���ý�����Ϣͨ��(��10��),����1024
    for (i = 30; i < 40; i++)
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
        fh228cfg->channels[i].dataByteCount  = 1024;
    }

    //4.NC->NT ���ý�����Ϣͨ��(��10��),����64*1024
    for (i = 40; i < 50; i++)
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
        fh228cfg->channels[i].dataByteCount  = 64*1024;
    }

    //5.NC->NT ���ý�����Ϣͨ��(��10��),����16M
    for (i = 50; i < 60; i++)
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
        fh228cfg->channels[i].dataByteCount  = 16*1024*1024;
    }
    
    //6.NT->NC ���÷�����Ϣͨ��(��10��)  ,����1024 
    for (i = 60; i < 70; i++)
    {
        fh228cfg->channels[i].subAddr        = i + 1;
        fh228cfg->channels[i].priority       = priority;
        fh228cfg->channels[i].sid            = fh228cfg->baseInfo.fcid;
        fh228cfg->channels[i].did            = ncFcId;
        fh228cfg->channels[i].otherDid       = 0x000000;
        fh228cfg->channels[i].otherSubAddr   = 0;
        fh228cfg->channels[i].suppressStatus = suppressStatus;
        fh228cfg->channels[i].retryNum       = 0;
        fh228cfg->channels[i].tr             = FH228_DATA_DIRECT_TX;
        fh228cfg->channels[i].dataByteCount  = 1024 ;
    }

    //7.NT->NC ���÷�����Ϣͨ��(��10��)  ,����64*1024
    for (i = 70; i < 80; i++)
    {
        fh228cfg->channels[i].subAddr        = i + 1;
        fh228cfg->channels[i].priority       = priority;
        fh228cfg->channels[i].sid            = fh228cfg->baseInfo.fcid;
        fh228cfg->channels[i].did            = ncFcId;
        fh228cfg->channels[i].otherDid       = 0x000000;
        fh228cfg->channels[i].otherSubAddr   = 0;
        fh228cfg->channels[i].suppressStatus = suppressStatus;
        fh228cfg->channels[i].retryNum       = 0;
        fh228cfg->channels[i].tr             = FH228_DATA_DIRECT_TX;
        fh228cfg->channels[i].dataByteCount  = 64*1024 ;
    }

//==========================������������Ϣ==================
    //8.NC->NT ����������Ϣ����ͨ��(��6��)  ,����1024
    for (i = 90; i < 96; i++)
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
        fh228cfg->channels[i].dataByteCount  = 1024;
    }

    //9.NT->NC ����������Ϣ����ͨ��(��6��)  ,����1024
    for (i = 96; i < 102; i++)
    {
        fh228cfg->channels[i].subAddr        = i + 1;
        fh228cfg->channels[i].priority       = priority;
        fh228cfg->channels[i].sid            = fh228cfg->baseInfo.fcid;
        fh228cfg->channels[i].did            = ncFcId;
        fh228cfg->channels[i].otherDid       = 0x000000;
        fh228cfg->channels[i].otherSubAddr   = 0;
        fh228cfg->channels[i].suppressStatus = suppressStatus;
        fh228cfg->channels[i].retryNum       = 0;
        fh228cfg->channels[i].tr             = FH228_DATA_DIRECT_TX;
        fh228cfg->channels[i].dataByteCount  = 1024 ;
    }

    //NT1������������Ϣ��NT2/NT3/NT4/NT5/NT6���������Ϣ
    if (myNtNo == 1)
    {
        did = 0x100001;
        //10.NT1->NT2/NT3/NT4/NT5/NT6 ����������Ϣ����ͨ��(��5��)  ,����1024
        for (i = 102; i < 107; i++)
        {
            fh228cfg->channels[i].subAddr        = i + 1;
            fh228cfg->channels[i].priority       = priority;
            fh228cfg->channels[i].sid            = fh228cfg->baseInfo.fcid;
            fh228cfg->channels[i].did            = ++did;
            fh228cfg->channels[i].otherDid       = 0x000000;
            fh228cfg->channels[i].otherSubAddr   = 0;
            fh228cfg->channels[i].suppressStatus = suppressStatus;
            fh228cfg->channels[i].retryNum       = 0;
            fh228cfg->channels[i].tr             = FH228_DATA_DIRECT_TX;
            fh228cfg->channels[i].dataByteCount  = 1024 ;
        }
    }
    else

    {
        //11.NT1->NT2/NT3/NT4/NT5/NT6 ���նˣ����ý�����Ϣͨ��(��5��ͨ��)
        for (i = 102; i < 107; i++)
        {
            fh228cfg->channels[i].subAddr        = i + 1;
            fh228cfg->channels[i].priority       = priority;
            fh228cfg->channels[i].sid            = fh228cfg->baseInfo.fcid;
            fh228cfg->channels[i].did            = ncFcId;
            fh228cfg->channels[i].otherDid       = 0x000000;
            fh228cfg->channels[i].otherSubAddr   = 0;
            fh228cfg->channels[i].suppressStatus = suppressStatus;
            fh228cfg->channels[i].retryNum       = 0;
            fh228cfg->channels[i].nt2nt          = 0;
            fh228cfg->channels[i].tr             = FH228_DATA_DIRECT_RX;
            fh228cfg->channels[i].dataByteCount  = 1024;
        }

    }
}

#if 0
void nc2nt(const int myNtNo, Fh228Config* fh228cfg, const int dataBytes, int start, int end)
{
    int i = 0;
        
    unsigned char *recvBuf = (unsigned char *)malloc(dataBytes);

    for (;;)
    {
        for (i = start; i < end; i++)
        {
            unsigned int sa = fh228cfg->channels[i].subAddr;
            int realLen = Fh228_Recv(devId, sa, (char*)recvBuf, dataBytes);
            if (realLen < 0)
            {
                elog_e(LOG_TAG, "error: recv data.len=%d", realLen);
                break;
            }
        }
    }
    elog_d(LOG_TAG, "set nc2nt finished");
    free(recvBuf);
}


void nt2nt_rnt(const int myNtNo, Fh228Config* fh228cfg, const int dataBytes, int start, int end)
{
    int i = 0;
    unsigned char *recvBuf = (unsigned char *)malloc(dataBytes);

    for (;;)
    {
        for (i = start; i < end; i++)
        {
            unsigned int sa = fh228cfg->channels[i].subAddr;
            int realLen = Fh228_Recv(devId, sa, (char*)recvBuf, dataBytes);
            if (realLen < 0)
            {
                elog_e(LOG_TAG, "error: recv data.len=%d", realLen);
                break;
            }
        }
    }

    elog_d(LOG_TAG, "test case finished");
    
    free(recvBuf);
}



// ʹ��91 - 120 �ӵ�ַ
// ������Ϣ������������


#endif






#if 0
void tmp()
{
    // ���ݲ�ͬ��NT�ڵ㣬���ò�ͬ������
        switch (myNtNo)
        {
            case 1:
               nextBaseAddr = 0;
               dataBytes = 1024;
               break;
            case 2:
               nextBaseAddr = 5;
               dataBytes = 1024;
               break;
            case 3:
               nextBaseAddr = 10;
               dataBytes = 1024;
               break;
            case 4:
               nextBaseAddr = 15;
               dataBytes = 64*1024;
               break;
            case 5:
               nextBaseAddr = 20;
               dataBytes = 64*1024;
               break;
            case 6:
               nextBaseAddr = 25;
               dataBytes = 64*1024;
               break;
        }

}
#endif

