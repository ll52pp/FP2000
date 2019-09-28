#ifndef STRUCT_H_14A9A4DF_1C2E_417A_99FA_A4D022683F48
#define STRUCT_H_14A9A4DF_1C2E_417A_99FA_A4D022683F48

#include "fpga_reg_addr.h"

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t address  : 16; ///< [ 0:15] : RW address
    uint32_t reserve1 : 13; ///< [16:28] : RW reserve
    uint32_t direct   : 1;  ///< [   29] : RW direct -- 0-RX 1-TX
    uint32_t reserve0 : 1;  ///< [   30] : RW reserve
    uint32_t action   : 1;  ///< [   31] : RW action   -- 0-read operation 1-write operation
#else
    uint32_t action   : 1;  ///< [   31] : RW action   -- 0-read operation 1-write operation
    uint32_t reserve0 : 1;  ///< [   30] : RW reserve
    uint32_t direct   : 1;  ///< [   29] : RW direct -- 0-RX 1-TX
    uint32_t reserve1 : 13; ///< [16:28] : RW reserve
    uint32_t address  : 16; ///< [ 0:15] : RW address
#endif
} IndirectCtrl;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t interval_index   : 7;  ///< [ 6: 0] : ʱ����������0~127����Ӧ128����ͬ����Ϣ���ͼ��
    uint32_t event_index      : 7;  ///< [13: 7] : ������0~127(����I / O����������������������ʱ��ȫ�������������������)������������������ʼֵ0~15
    uint32_t jmp_index        : 11; ///< [24:14] : ��Ϣ������(0~1023)����ת��ַ(0~2047)
    uint32_t reserved         : 2;  ///< [26:25] : ����
    /**
     * [29:27] : ָ�����ݣ�
     * 0x0 : ����������Ϣ;
     * 0x1 : ʱ������(��Ч)������Ϣ;
     * 0x2 : ������Ч������Ϣ;
     * 0x3 : ���߶���Ч�ŷ���(AND);
     * 0x4 : �κ�һ����Ч����(OR);
     * 0x5 : ������Ч����ת
     * 0x6 : ������������ת
     * 0x7 : ������������ת
     */
    uint32_t content          : 3;
    uint32_t inst_type        : 1; ///< [   30] : ָ�����ͣ�0x0 : ����ִ��ָ�� 0x1 : one-shotָ���ִ�к���FPGA�Զ����ָ��ʹ��λ
    uint32_t enab             : 1; ///< [   31] : ָ��ʹ��
#else
    uint32_t enab             : 1; ///< [   31] : ָ��ʹ��*/
    uint32_t inst_type        : 1; ///< [   30] : ָ�����ͣ�0x0 : ����ִ��ָ�� 0x1 : one-shotָ���ִ�к���FPGA�Զ������ָ��ʹ�ܡ�λ*/
    /**
     * [29:27] : ָ�����ݣ�
     * 0x0 : ����������Ϣ;
     * 0x1 : ʱ������(��Ч)������Ϣ;
     * 0x2 : ������Ч������Ϣ;
     * 0x3 : ���߶���Ч�ŷ���(AND);
     * 0x4 : �κ�һ����Ч����(OR);
     * 0x5 : ������Ч����ת
     * 0x6 : ������������ת
     * 0x7 : ������������ת
     */
    uint32_t content          : 3;
    uint32_t reserved         : 2;  ///< [26:25] : ����
    uint32_t jmp_index        : 11; ///< [24:14] : ��Ϣ������(0~1023)����ת��ַ(0~2047)
    uint32_t event_index      : 7;  ///< [13: 7] : ������0~127(����I / O����������������������ʱ��ȫ�������������������)������������������ʼֵ0~15
    uint32_t interval_index   : 7;  ///< [ 6: 0] : ʱ����������0~127����Ӧ128����ͬ����Ϣ���ͼ��
#endif
} Fc1553R0PeriodInst;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t did   : 24; ///< [ 0:23] : DID
    uint32_t r_ctl : 8;  ///< [24:31] : R_CTL
#else
    uint32_t r_ctl : 8;  ///< [24:31] : R_CTL
    uint32_t did   : 24; ///< [ 0:23] : DID
#endif
} FcDidRCtl;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t sid             : 24; ///< [ 0:23] : SID
    uint32_t preemption      : 1;  ///< [   24] : preemption
    uint32_t cs_ctl_priority : 7;  ///< [25:31] : CS_CTL/Priority
#else
    uint32_t cs_ctl_priority : 7;  ///< [25:31] : CS_CTL/Priority
    uint32_t preemption      : 1;  ///< [   24] : preemption
    uint32_t sid             : 24; ///< [ 0:23] : SID
#endif
} FcSidCsCtlPriority;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t fill_byte : 2;  ///< [ 0: 1] : fill byte
    uint32_t f_ctl     : 22; ///< [ 2:23] : F_CTL
    uint32_t type      : 8;  ///< [24:31] : TYPE
#else
    uint32_t type      : 8;  ///< [24:31] : TYPE
    uint32_t f_ctl     : 22; ///< [ 2:23] : F_CTL
    uint32_t fill_byte : 2;  ///< [ 0: 1] : fill byte
#endif
} FcFCtlType;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t multicast                     : 1;  ///< [    0] : Multicast
    uint32_t nc_monitor                    : 1;  ///< [    1] : NC Monitor for NT-to-NT Transfer
    uint32_t tr                            : 1;  ///< [    2] : T/R*
    uint32_t nt_to_nt_transfer             : 1;  ///< [    3] : NT-to-NT Transfer
    uint32_t suppress_status               : 1;  ///< [    4] : Suppress Status
    uint32_t transmit_rdma                 : 1;  ///< [    5] : Transmit RDMA
    uint32_t receive_rdma                  : 1;  ///< [    6] : Receive RDMA
    uint32_t delayed_nt_burst_size_request : 1;  ///< [    7] : Delayed NT Burst Size Request
    uint32_t nt_burst_size_request         : 1;  ///< [    8] : NT Burst Size Request
    uint32_t reserved                      : 23; ///< [ 9: 31]: Reserved
#else
    uint32_t reserved                      : 23; ///< [ 9: 31]: Reserved
    uint32_t nt_burst_size_request         : 1;  ///< [    8] : NT Burst Size Request
    uint32_t delayed_nt_burst_size_request : 1;  ///< [    7] : Delayed NT Burst Size Request
    uint32_t receive_rdma                  : 1;  ///< [    6] : Receive RDMA
    uint32_t transmit_rdma                 : 1;  ///< [    5] : Transmit RDMA
    uint32_t suppress_status               : 1;  ///< [    4] : Suppress Status
    uint32_t nt_to_nt_transfer             : 1;  ///< [    3] : NT-to-NT Transfer
    uint32_t tr                            : 1;  ///< [    2] : T/R*
    uint32_t nc_monitor                    : 1;  ///< [    1] : NC Monitor for NT-to-NT Transfer
    uint32_t multicast                     : 1;  ///< [    0] : Multicast
#endif
} FcAc1553CommandSeqWord6;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t multicast_address_other_port_id : 24; ///< [ 0:23] : Mulicast Address or Other Port_ID
    uint32_t reserved : 8; ///< [24:31] : Reserved
#else
    uint32_t reserved : 8; ///< [24:31] : Reserved
    uint32_t multicast_address_other_port_id : 24; ///< [ 0:23] : Mulicast Address or Other Port_ID
#endif
} FcAe1553CommandSeqWord10;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t seq_cnt : 16; ///< [ 0:15] : SEQ_CNT
    uint32_t df_ctl  : 8;  ///< [16:23] : DF_CTL
    uint32_t seq_id  : 8;  ///< [24:31] : SEQ_ID
#else
    uint32_t seq_id  : 8;  ///< [24:31] : SEQ_ID
    uint32_t df_ctl  : 8;  ///< [16:23] : DF_CTL
    uint32_t seq_cnt : 16; ///< [ 0:15] : SEQ_CNT
#endif
} FcSeqCntDfCtlSeqId;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t rx_id : 16; ///< [ 0:15] : RX_ID
    uint32_t ox_id : 16; ///< [16:31] : OX_ID
#else
    uint32_t ox_id : 16; ///< [16:31] : OX_ID
    uint32_t rx_id : 16; ///< [ 0:15] : RX_ID
#endif
} FcRxIdOxId;

#endif
