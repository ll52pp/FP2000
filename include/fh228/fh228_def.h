#ifndef FH228_DEF_H_6687B536_6B40_4500_88B7_B5436D671E6C
#define FH228_DEF_H_6687B536_6B40_4500_88B7_B5436D671E6C

#define FH228_MAX_PERIODIC_MSG_COUNT                  2048      ///< 最大周期消息个数
#define FH228_MAX_CHANNEL_COUNT                       1024      ///< 最大消息通道个数
#define FH228_MAX_PERIODIC_MSG_TIME_INTERVAL_COUNT    128       ///< 一共能配置多少种不同的时间间隔
#define FH228_MAX_PERIODIC_MSG_CMD_COUNT              2048      ///< 周期指令最大个数
#define FH228_MAX_SEND_BYTES_LOW                      16777216  ///< 低优先级所能发送最大的单个包字节数, 大于这个数, 需要切包再发送
#define FH228_MAX_SEND_BYTES_HIGH                     8384512   ///< 高优先级所能发送最大的单个包字节数, 大于这个数, 需要切包再发送

// FH228的角色
typedef enum
{
    FH228_ROLE_NT = 0x00000000, ///< NT端
    FH228_ROLE_NC = 0x00000001, ///< NC端
} Fh228Role;

// 速率
typedef enum
{
    FH228_SPEED_1G = 0x00000001, ///< 1G
    FH228_SPEED_2G = 0x00000002, ///< 2G
    FH228_SPEED_4G = 0x00000003, ///< 4G
} Fh228Speed;

// 优先级
typedef enum
{
    FH228_PRIORITY_LOW   = 0x00000000, ///< 低优先级
    FH228_PRIORITY_MID   = 0x00000001, ///< 中优先级
    FH228_PRIORITY_HIGH  = 0x00000002, ///< 高优先级
    FH228_PRIORITY_COUNT = 0x00000003, ///< 优先级个数
} Fh228Priority;

/**
 * 数据方向T/R*
 *
 * 1. 对于NC端来说, T是发数据, R是发命令,收数据
 * 2. 对于NT端来说, T是预配数据, R是收数据
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

// FH228的设备基本参数
typedef struct
{
    Fh228Role    role;                 ///< 角色, 0-NC, 1NT
    unsigned int fcid;                 ///< FCID
    unsigned int creditNum;            ///< 信用，1-16
    Fh228Speed   speed;                ///< 速率 1-1Gbps 2-2Gbps 3-4Gbps
    unsigned int edtov;                ///< 超时时间，单位是毫秒
    unsigned int isRedundantEnabled;   ///< 冗余使能 0-禁止冗余 1-使能冗余
    unsigned int ntTimeoutMicrosec;    ///< NT超时时间，单位是微秒
    unsigned int nt2ntTimeoutMicrosec; ///< NT2NT超时时间，单位是微秒
} Fh228BaseInfo;

// 各消息通道的属性值
typedef struct
{
    unsigned int    subAddr;        ///< 子地址, 范围(0, 0xffffffff)
    Fh228Priority   priority;       ///< 优先级, 范围[0,2], 0优先级最低
    unsigned int    sid;            ///< 本地FCID, 如0x100000
    unsigned int    did;            ///< 目标FCID, 如0x200000
    unsigned int    otherDid;       ///< 第三台设备的FCID, 如0x300000
    unsigned int    otherSubAddr;   ///< 第三台设备的子地址
    unsigned int    suppressStatus; ///< 状态抑制 0-回复状态包 1-不回复状态包
    unsigned int    retryNum;       ///< 重试次数 0-2
    unsigned int    nt2nt;          ///< nt2nt
    Fh228DataDirect tr;             ///< T/R*, 0表示T, 1表示R*
    unsigned int    dataByteCount;  ///< 通道数据大小
} Fh228ChannelProperty;

// 周期指令
typedef struct
{
    int isEnabled; ///< 使能, 1启用, 0禁用
    int type; ///< 指令类型, 0:持续执行指令, 1:one-shot指令，执行后FPGA自动清除使能
    /**
     * 内容字段: 取值如下
     * 0:立即发送消息
     * 1:时间间隔到(有效)后发送消息
     * 2:条件有效后发送消息
     * 3:
     * 4:
     * 5:
     * 6:无条件计数跳转
     * 7:无条件立即跳转
     */
    int content;
    int jumpIndex; ///< 消息索引号（0~1023）或跳转地址（0~2047）
    int timeInterval; ///< 时间间隔
} Fh228PeriodicMsgCmd;

// 设备的配置信息
typedef struct
{
    Fh228BaseInfo        baseInfo;
    int                  channelCount;
    Fh228ChannelProperty channels[FH228_MAX_CHANNEL_COUNT];
} Fh228Config;

#endif
