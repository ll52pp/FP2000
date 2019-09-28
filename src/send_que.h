#ifndef WRITE_IO_LIST_H_EE9D57D7_8D60_4808_97D6_5BC5211D396E
#define WRITE_IO_LIST_H_EE9D57D7_8D60_4808_97D6_5BC5211D396E

#include "common_header.h"
#include "struct.h"

#define sendQueGetDmaBufSize(context) ((context)->dmaSize)

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t length : 25; ///< [ 0:24] : RW length
    uint32_t eop : 1;  ///< [   25] : RW eop
    uint32_t sop : 1;  ///< [   26] : RW sop
    uint32_t reserved : 4;  ///< [27:30] : RW reserved
    uint32_t flag : 1;  ///< [   31] : RW sw set 0, fpga set 1
#else
    uint32_t flag : 1;  ///< [   31] : RW sw set 0, fpga set 1
    uint32_t reserved : 4;  ///< [27:30] : RW reserved
    uint32_t sop : 1;  ///< [   26] : RW sop
    uint32_t eop : 1;  ///< [   25] : RW eop
    uint32_t length : 25; ///< [ 0:24] : RW length
#endif
} W0;

typedef struct
{
    uint32_t length; ///< iu length
} W1;

typedef struct
{
    W0 w0;
    W1 w1;
    char data[1];
} SendQueDmaNode;

typedef struct
{
    CS_RING_BUF_NODE_OBJECT;
    int index; ///< �±�, ��0��ʼ
    char* rawDmaBuf; ///< DMA buf���׵�ַ, ��Ϊdma��ַ��Ҫ����1k����, ������Ҫ����ֶα����׵�ַ,�ͷŵ�ʱ����
    SendQueDmaNode* dmaBuf; ///< DMA���ݵ�ַ, ������ֶ�_rawDmaBuf����1k������dma��ַ
} SendQueNode;

typedef struct
{
    CsRingBufContext ringList; ///< ���λ�����
    int dmaCnt; ///< DMA buffer����
    int dmaSize; ///< ÿ���ڵ��DMA buffer��С
    BaMutexContext lock;
} SendQueContext;

void SendQue_Init(SendQueContext* context, int dmaBufCnt, int dmaBufSize, int msgNumBase);
void SendQue_Uninit(SendQueContext* context);
void SendQue_Lock(SendQueContext* context);
void SendQue_Unlock(SendQueContext* context);
int  SendQue_GetNodeCount(SendQueContext* context);
int  SendQue_GetNodeSize(SendQueContext* context);
void SendQue_PushBack(SendQueContext* context, SendQueNode* node);
void SendQue_Move2NextNode(SendQueContext* context);
bool SendQue_IsNodeWriteable(SendQueNode* node);
void SendQue_SetNodeNonWriteable(SendQueNode* node, const int iuLength, const int sublength, const int sop, const int eop);
SendQueNode* SendQue_PeekFront(SendQueContext* context);

#endif
