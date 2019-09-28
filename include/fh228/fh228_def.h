#ifndef FH228_DEF_H_6687B536_6B40_4500_88B7_B5436D671E6C
#define FH228_DEF_H_6687B536_6B40_4500_88B7_B5436D671E6C

#define FH228_MAX_PERIODIC_MSG_COUNT                  2048      ///< ���������Ϣ����
#define FH228_MAX_CHANNEL_COUNT                       1024      ///< �����Ϣͨ������
#define FH228_MAX_PERIODIC_MSG_TIME_INTERVAL_COUNT    128       ///< һ�������ö����ֲ�ͬ��ʱ����
#define FH228_MAX_PERIODIC_MSG_CMD_COUNT              2048      ///< ����ָ��������
#define FH228_MAX_SEND_BYTES_LOW                      16777216  ///< �����ȼ����ܷ������ĵ������ֽ���, ���������, ��Ҫ�а��ٷ���
#define FH228_MAX_SEND_BYTES_HIGH                     8384512   ///< �����ȼ����ܷ������ĵ������ֽ���, ���������, ��Ҫ�а��ٷ���

// FH228�Ľ�ɫ
typedef enum
{
    FH228_ROLE_NT = 0x00000000, ///< NT��
    FH228_ROLE_NC = 0x00000001, ///< NC��
} Fh228Role;

// ����
typedef enum
{
    FH228_SPEED_1G = 0x00000001, ///< 1G
    FH228_SPEED_2G = 0x00000002, ///< 2G
    FH228_SPEED_4G = 0x00000003, ///< 4G
} Fh228Speed;

// ���ȼ�
typedef enum
{
    FH228_PRIORITY_LOW   = 0x00000000, ///< �����ȼ�
    FH228_PRIORITY_MID   = 0x00000001, ///< �����ȼ�
    FH228_PRIORITY_HIGH  = 0x00000002, ///< �����ȼ�
    FH228_PRIORITY_COUNT = 0x00000003, ///< ���ȼ�����
} Fh228Priority;

/**
 * ���ݷ���T/R*
 *
 * 1. ����NC����˵, T�Ƿ�����, R�Ƿ�����,������
 * 2. ����NT����˵, T��Ԥ������, R��������
 * |   | NC                | NT           |
 * |---|-------------------|--------------|
 * | T | send data         | preplan data |
 * | R | send cmd,recv data| recv data    |
 */
typedef enum
{
    FH228_DATA_DIRECT_TX = 0x00000000, ///< T
    FH228_DATA_DIRECT_RX = 0x00000001, ///< R
} Fh228DataDirect;

// FH228���豸��������
typedef struct
{
    Fh228Role    role;                 ///< ��ɫ, 0-NC, 1NT
    unsigned int fcid;                 ///< FCID
    unsigned int creditNum;            ///< ���ã�1-16
    Fh228Speed   speed;                ///< ���� 1-1Gbps 2-2Gbps 3-4Gbps
    unsigned int edtov;                ///< ��ʱʱ�䣬��λ�Ǻ���
    unsigned int isRedundantEnabled;   ///< ����ʹ�� 0-��ֹ���� 1-ʹ������
    unsigned int ntTimeoutMicrosec;    ///< NT��ʱʱ�䣬��λ��΢��
    unsigned int nt2ntTimeoutMicrosec; ///< NT2NT��ʱʱ�䣬��λ��΢��
} Fh228BaseInfo;

// ����Ϣͨ��������ֵ
typedef struct
{
    unsigned int    subAddr;        ///< �ӵ�ַ, ��Χ(0, 0xffffffff)
    Fh228Priority   priority;       ///< ���ȼ�, ��Χ[0,2], 0���ȼ����
    unsigned int    sid;            ///< ����FCID, ��0x100000
    unsigned int    did;            ///< Ŀ��FCID, ��0x200000
    unsigned int    otherDid;       ///< ����̨�豸��FCID, ��0x300000
    unsigned int    otherSubAddr;   ///< ����̨�豸���ӵ�ַ
    unsigned int    suppressStatus; ///< ״̬���� 0-�ظ�״̬�� 1-���ظ�״̬��
    unsigned int    retryNum;       ///< ���Դ��� 0-2
    unsigned int    nt2nt;          ///< nt2nt
    Fh228DataDirect tr;             ///< T/R*, 0��ʾT, 1��ʾR*
    unsigned int    dataByteCount;  ///< ͨ�����ݴ�С
} Fh228ChannelProperty;

// ����ָ��
typedef struct
{
    int isEnabled; ///< ʹ��, 1����, 0����
    int type; ///< ָ������, 0:����ִ��ָ��, 1:one-shotָ�ִ�к�FPGA�Զ����ʹ��
    /**
     * �����ֶ�: ȡֵ����
     * 0:����������Ϣ
     * 1:ʱ������(��Ч)������Ϣ
     * 2:������Ч������Ϣ
     * 3:
     * 4:
     * 5:
     * 6:������������ת
     * 7:������������ת
     */
    int content;
    int jumpIndex; ///< ��Ϣ�����ţ�0~1023������ת��ַ��0~2047��
    int timeInterval; ///< ʱ����
} Fh228PeriodicMsgCmd;

// �豸��������Ϣ
typedef struct
{
    Fh228BaseInfo        baseInfo;
    int                  channelCount;
    Fh228ChannelProperty channels[FH228_MAX_CHANNEL_COUNT];
} Fh228Config;

#endif
