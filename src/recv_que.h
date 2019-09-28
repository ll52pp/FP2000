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
        UINT32 subAddr; ///< 只有第一包有这个字段, 其它包没有, 用于标识此包所属的子地址
        char ptr[1]; ///< 数据首地址
    } data; ///< 这个地方, 在第一个包中, 使用的是subAddr字段, 用来区别子地址, 非第一个包中, 用于存储数据首地址
} RecvQueDmaNode;

typedef struct
{
    CS_RING_BUF_NODE_OBJECT;
    int index; /// 下标, 从0开始
    char* rawDmaBuf; ///< DMA buf的首地址, 因为dma地址需要进行1k对齐, 所以需要这个字段保存首地址,释放的时候用
    RecvQueDmaNode* dmaBuf; ///< DMA数据地址, 相对于字段_rawDmaBuf进行1k对齐后的dma地址
} RecvQueNode;

typedef struct
{
    CsRingBufContext ringList;
    BaMutexContext _mutex;
    int dmaCnt; // DMA buffer个数
    int dmaSize; // 每个节点的DMA buffer大小
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
