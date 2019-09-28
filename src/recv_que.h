#ifndef READ_IO_LIST_H_EE9D57D7_8D60_4808_97D6_5BC5211D396E
#define READ_IO_LIST_H_EE9D57D7_8D60_4808_97D6_5BC5211D396E

#include "common_header.h"
#include "struct.h"

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t length    : 21;/*[ 0:20] : RW valid data length in buffer*/
    uint32_t reserved  : 6; /*[21:26] : RW reserved*/
    uint32_t frame_err : 1; /*[   27] : RW frame error*/
    uint32_t ox_id_err : 1; /*[   28] : RW ox_id error*/
    uint32_t seq_err   : 1; /*[   29] : RW seq error*/
    uint32_t eop       : 1; /*[   30] : RW eop*/
    uint32_t sop       : 1; /*[   31] : RW sop*/
#else
    uint32_t sop       : 1; /*[   31] : RW sop*/
    uint32_t eop       : 1; /*[   30] : RW eop*/
    uint32_t seq_err   : 1; /*[   29] : RW seq error*/
    uint32_t ox_id_err : 1; /*[   28] : RW ox_id error*/
    uint32_t frame_err : 1; /*[   27] : RW frame error*/
    uint32_t reserved  : 6; /*[21:26] : RW reserved*/
    uint32_t length    : 21;/*[ 0:20] : RW valid data length in buffer*/
#endif
} R0;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t reserved : 22; ///< [ 0:21] : RW reserved
    uint32_t msg_num  : 10; ///< [22:31] : RW message number
#else
    uint32_t msg_num  : 10; ///< [22:31] : RW message number
    uint32_t reserved : 22; ///< [ 0:21] : RW reserved
#endif
} R1;

typedef struct
{
    R0 r0;
    R1 r1;
    union
    {
        UINT32 subAddr; ///< ֻ�е�һ��������ֶ�, ������û��, ���ڱ�ʶ�˰��������ӵ�ַ
        char ptr[1]; ///< �����׵�ַ
    } data; ///< ����ط�, �ڵ�һ������, ʹ�õ���subAddr�ֶ�, ���������ӵ�ַ, �ǵ�һ������, ���ڴ洢�����׵�ַ
} RecvQueDmaNode;

typedef struct
{
    CS_RING_BUF_NODE_OBJECT;
    int index; /// �±�, ��0��ʼ
    char* rawDmaBuf; ///< DMA buf���׵�ַ, ��Ϊdma��ַ��Ҫ����1k����, ������Ҫ����ֶα����׵�ַ,�ͷŵ�ʱ����
    RecvQueDmaNode* dmaBuf; ///< DMA���ݵ�ַ, ������ֶ�_rawDmaBuf����1k������dma��ַ
} RecvQueNode;

typedef struct
{
    CsRingBufContext ringList;
    BaMutexContext _mutex;
    int dmaCnt; // DMA buffer����
    int dmaSize; // ÿ���ڵ��DMA buffer��С
} RecvQueContext;

void RecvQue_Init(RecvQueContext* context, int dmaBufCnt, int dmaBufSize, int startOfIndex);
void RecvQue_Uninit(RecvQueContext* context);

int  RecvQue_GetDmaBufSize(RecvQueContext* context);
void RecvQue_PushBack(RecvQueContext* context, RecvQueNode* node);
void RecvQue_Move2NextNode(RecvQueContext* context);
RecvQueNode* RecvQue_PeekFront(RecvQueContext* context);
RecvQueNode* RecvQue_PopFront(RecvQueContext* context);

bool RecvQue_IsNodeReadable(RecvQueNode* node);
void RecvQue_SetNodeEmpty(RecvQueNode* node);
bool RecvQue_IsNodeR0Ok(RecvQueNode* node);
#endif
