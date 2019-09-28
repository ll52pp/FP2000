#ifndef __FH228_HW_H__
#define __FH228_HW_H__

#include "common_header.h"
#include "struct.h"
#include "config_mgr.h"

typedef struct
{
#ifndef __BIG_ENDIAN__
        uint32_t sid             : 24; ///< [ 0:23] : RW sid               -- self id
        uint32_t credit          : 4;  ///< [24:27] : RW credit            -- set 0～15 -- 1～16 number of credit
        uint32_t speed           : 2;  ///< [28:29] : RW speed             -- 00 auto sense 01 1Gbps 10 2Gbps 11 4Gbps
        uint32_t did_filter_enab : 1;  ///< [30   ] : RW did filter enable -- 0-enable 1-disable
        uint32_t reserve         : 1;  ///< [31   ] : RW reserve           --
#else
        uint32_t reserve         : 1;  ///< [31   ] : RW reserve           --
        uint32_t did_filter_enab : 1;  ///< [30   ] : RW did filter enable -- 0-enable 1-disable
        uint32_t speed           : 2;  ///< [28:29] : RW speed             -- 00 auto sense 01 1Gbps 10 2Gbps 11 4Gbps
        uint32_t credit          : 4;  ///< [24:27] : RW credit            -- set 0～15 -- 1～16 number of credit
        uint32_t sid             : 24; ///< [ 0:23] : RW sid               -- self id
#endif
} PortCfg;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t port_enab       : 1;  ///< [ 0   ] : RW port0 RX enab port0 rx enable 0-disable rx 1-enable rx
    uint32_t mac_switch      : 1;  ///< [ 1   ] : RW port0 Mac force off 0-mac on 1-mac off
    uint32_t reset           : 1;  ///< [ 2   ] : RW stop以后，qdr buffer空
    uint32_t reserve         : 10; ///< [ 3:12] : RW reserve
    uint32_t raw_priority    : 2;  ///< [13:14] : RW RW tx raw 使用的优先级 0-发送低优先级 1-    2-发送高优先级
    uint32_t redundancy_enab : 1;  ///< [15   ] : R Redundant enable '0' disable '1' enable
    uint32_t edtov           : 16; ///< [16:31] : R E_d_tov value, unit is 1ms
#else
    uint32_t edtov           : 16; ///< [16:31] : RW E_d_tov value, unit is 1ms
    uint32_t redundancy_enab : 1;  ///< [15   ] : RW Redundant enable '0' disable '1' enable
    uint32_t raw_priority    : 2;  ///< [13:14] : RW RW tx raw 使用的优先级 0-发送低优先级 1-    2-发送高优先级
    uint32_t reserve         : 10; ///< [ 3:12] : RW reserve
    uint32_t reset           : 1;  ///< [ 2   ] : RW stop以后，qdr buffer空
    uint32_t mac_switch      : 1;  ///< [ 1   ] : RW port0 Mac force off -- 0-mac on 1-mac off
    uint32_t port_enab       : 1;  ///< [ 0   ] : RW port0 RX enab       -- port0 rx enable 0-disable rx 1-enable rx
#endif
} PortEnab;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t event_period          : 16; ///< [ 0:15] : RW event period unit is ms, Note: The actual event period equals the programmed value plus one.
    uint32_t p0_sync_enab          : 1;  ///< [16   ] : RW port0 sync enable   -- 0-disable 1-enable
    uint32_t p1_sync_enab          : 1;  ///< [17   ] : RW port1 sync enable   -- 0-disable 1-enable
    uint32_t rtc_base              : 1;  ///< [18   ] : RW rtc base -- 0-5x  division, used in 1G/2G mode 1-10x division, used in 2G/4G mode
    uint32_t global_sync_enab      : 1;  ///< [19   ] : RW hba global sync enable -- 0-disable 1-enable
    uint32_t rtc_write_enab        : 1;  ///< [20   ] : RW write rtc enable -- 0-disable 1-enable
    uint32_t reserve               : 11; ///< [21:31] : RW reserve
#else
    uint32_t reserve               : 11; ///< [21:31] : RW reserve
    uint32_t rtc_write_enab        : 1;  ///< [20   ] : RW write rtc enable -- 0-disable 1-enable
    uint32_t global_sync_enab      : 1;  ///< [19   ] : RW hba global sync enable    -- 0-disable 1-enable
    uint32_t rtc_base              : 1;  ///< [18   ] : RW rtc base -- 0-5x  division, used in 1G/2G mode 1-10x division, used in 2G/4G mode
    uint32_t p1_sync_enab          : 1;  ///< [17   ] : RW port1 sync enable   -- 0-disable 1-enable
    uint32_t p0_sync_enab          : 1;  ///< [16   ] : RW port0 sync enable   -- 0-disable 1-enable
    uint32_t event_period          : 16; ///< [ 0:15] : RW event period unit is ms, Note: The actual event period equals the programmed value plus one.
#endif
} RtcCtrl;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t p0_status   : 1;  ///< [ 0   ] : RO port0 status -- 0-loss of sync 1-sync
    uint32_t p1_status   : 1;  ///< [ 1   ] : RO port1 status -- 0-loss of sync 1-sync
    uint32_t reset_done  : 1;  ///< [ 2   ] : RO reset done
    uint32_t reserve     : 5;  ///< [ 3: 7] : RW reserve
    uint32_t voltage     : 12; ///< [ 8:19] : RO fpga voltage
    uint32_t temperature : 12; ///< [20:31] : RO fpga temperature
#else
    uint32_t temperature : 12; ///< [20:31] : RO fpga temperature
    uint32_t voltage     : 12; ///< [ 8:19] : RO fpga voltage
    uint32_t reserve     : 5;  ///< [ 3: 7] : RW reserve
    uint32_t reset_done  : 1;  ///< [ 2   ] : RO reset done
    uint32_t p1_status   : 1;  ///< [ 1   ] : RO port1 status -- 0-loss of sync 1-sync
    uint32_t p0_status   : 1;  ///< [ 0   ] : RO port0 status -- 0-loss of sync 1-sync
#endif
} PortState;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t value   : 10; ///< [ 0: 9] RW rtc high bit [32:41]
    uint32_t reserve : 22; ///< [10:31] RW reserve
#else
    uint32_t reserve : 22; ///< [10:31] RW reserve
    uint32_t value   : 10; ///< [ 0: 9] RW rtc high bit [32:41]
#endif
} RtcTimeWriteH;

typedef uint32_t RtcTimeWriteL; ///< [ 0:31] RW rtc low bit [ 0:31]

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t value   : 10; ///< [ 0: 9] RW rtc high bit [32:41]
    uint32_t reserve : 22; ///< [10:31] RW reserve
#else
    uint32_t reserve : 22; ///< [10:31] RW reserve
    uint32_t value   : 10; ///< [ 0: 9] RW rtc high bit [32:41]
#endif
} RtcTimeReadH;

typedef uint32_t RtcTimeReadL; // [ 0:31] RW rtc low bit [ 0:31]

typedef struct
{
#ifndef __BIG_ENDIAN__
    /**
     * [0 : 3] RW 0-cur timestamp 32'b~41'b
     * 1-cur timestamp 0'b~31'b
     * 2-recv word num
     * 3-recv idle num
     * 4-recv frame num
     * 5-recv word num
     * 6-recv idle num
     * 7-recv frame num
     */
    uint32_t type    : 6;
    uint32_t reserve : 25; ///< [ 4:30] RW
    uint32_t start   : 1;  ///< [   31] RW set 1 start statistic, self clear
#else
    uint32_t start   : 1;  ///< [   31] RW set 1 start statistic, self clear
    uint32_t reserve : 25; ///< [ 4:30] RW
    /**
     * [0 : 3] RW 0-cur timestamp 32'b~41'b
     * 1-cur timestamp 0'b~31'b
     * 2-recv word num
     * 3-recv idle num
     * 4-recv frame num
     * 5-recv word num
     * 6-recv idle num
     * 7-recv frame num
     */
    uint32_t type    : 6;
#endif
} StatisticsCtrl;

typedef uint32_t StatisticsData;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t time    : 24; ///< [ 0:23] : RW timeout, unit 1ms nt2nt=0
    uint32_t reserve : 8;  ///< [24:31] : RW reserve
#else
    uint32_t reserve : 8;  ///< [24:31] : RW reserve
    uint32_t time    : 24; ///< [ 0:23] : RW timeout, unit 1ms nt2nt=0
#endif
} NtTimeout;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t time    : 24; ///< [ 0:23] : RW timeout, unit 1ms nt2nt=1
    uint32_t reserve : 8;  ///< [24:31] : RW reserve
#else
    uint32_t reserve : 8;  ///< [24:31] : RW reserve
    uint32_t time    : 24; ///< [ 0:23] : RW timeout, unit 1ms nt2nt=1
#endif
} Nt2NtTimeout;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t flash_addr     : 24; ///< [ 0:23] :RW
    uint32_t reserve        : 4;  ///< [24:27] :RW
    uint32_t rd_flash_debug : 1;  ///< [28   ] :RW
    uint32_t rd_flash       : 1;  ///< [29   ] :RW read flash enable  -- 1-enable 0-disable
    uint32_t wr_flash_debug : 1;  ///< [30   ] :RW
    uint32_t wr_flash       : 1;  ///< [31   ] :RW write flash enable  -- 1-enable 0-disable
#else
    uint32_t wr_flash       : 1;  ///< [31   ] :RW write flash enable  -- 1-enable 0-disable
    uint32_t wr_flash_debug : 1;  ///< [30   ] :RW
    uint32_t rd_flash       : 1;  ///< [29   ] :RW read flash enable  -- 1-enable 0-disable
    uint32_t rd_flash_debug : 1;  ///< [28   ] :RW
    uint32_t reserve        : 4;  ///< [24:27] :RW
    uint32_t flash_addr     : 24; ///< [ 0:23] :RW
#endif
} FlashCtrl;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t pending_Rx_IU             : 1;  ///< [0   ] : RW
    uint32_t pending_Rx_credit_out     : 1;  ///< [1   ] : RW
    uint32_t pending_Tx_IU             : 1;  ///< [2   ] : RW
    uint32_t pending_send_bd_empty     : 1;  ///< [3   ] : RW
    uint32_t pending_link_change       : 1;  ///< [4   ] : RW
    uint32_t reserved                  : 21; ///< [5:25] : RW
    uint32_t intr_enable_Rx_IU         : 1;  ///< [26  ] : RW
    uint32_t intr_enable_Rx_credit_out : 1;  ///< [27  ] : RW
    uint32_t intr_enable_Tx_IU         : 1;  ///< [28  ] : RW
    uint32_t intr_enable_send_bd_empty : 1;  ///< [29  ] : RW
    uint32_t intr_enable_link_change   : 1;  ///< [30  ] : RW
    uint32_t intr_enable_global        : 1;  ///< [31  ] : RW
#else
    uint32_t intr_enable_global        : 1;  ///< [31  ] : RW
    uint32_t intr_enable_link_change   : 1;  ///< [30  ] : RW
    uint32_t intr_enable_send_bd_empty : 1;  ///< [29  ] : RW
    uint32_t intr_enable_Tx_IU         : 1;  ///< [28  ] : RW
    uint32_t intr_enable_Rx_credit_out : 1;  ///< [27  ] : RW
    uint32_t intr_enable_Rx_IU         : 1;  ///< [26  ] : RW
    uint32_t reserved                  : 21; ///< [5:25] : RW
    uint32_t pending_link_change       : 1;  ///< [4   ] : RW
    uint32_t pending_send_bd_empty     : 1;  ///< [3   ] : RW
    uint32_t pending_Tx_IU             : 1;  ///< [2   ] : RW
    uint32_t pending_Rx_credit_out     : 1;  ///< [1   ] : RW
    uint32_t pending_Rx_IU             : 1;  ///< [0   ] : RW
#endif
} InterruptCtrl;

typedef uint32_t RtcDateWrite; // [ 0:31] RW rtc day

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t real_time_day : 16; ///< [ 0:15] : RO real time day
    uint32_t reserve       : 15; ///< [16:30] : RO reserve
    uint32_t snap_shot_cmd : 1;  ///< [31   ] : RW snap shot command 1-启动快照
#else
    uint32_t snap_shot_cmd : 1;  ///< [31   ] : RW snap shot command 1-启动快照
    uint32_t reserve       : 15; ///< [16:30] : RO reserve
    uint32_t real_time_day : 16; ///< [ 0:15] : RO real time day
#endif
} RtcDateRead;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t r_a_tov_time_out : 24; ///< [ 0:23] : RW r_a_tov timeout unit 1ms
    uint32_t reserved         : 8;  ///< [24:31] : RW reserved
#else
    uint32_t reserved         : 8;  ///< [24:31] : RW reserved
    uint32_t r_a_tov_time_out : 24; ///< [ 0:23] : RW r_a_tov timeout unit 1ms
#endif
} RATovTimeout;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t fcid        : 24; ///< [ 0:23] : RW switch fcid
    uint32_t reserved    : 8;  ///< [24:31] : RW reserved
#else
    uint32_t reserved    : 8;  ///< [24:31] : RW reserved
    uint32_t fcid        : 24; ///< [ 0:23] : RW switch fcid
#endif
} SwitchFcId;

typedef struct
{
#ifndef __BIG_ENDIAN__
    uint32_t performance : 4;  ///< [ 0: 3] : RW 0~e, 值越小，发送性能越低
    uint32_t reserved    : 28; ///< [ 4:31] : RW reserved
#else
    uint32_t reserved    : 28; ///< [ 4:31] : RW reserved
    uint32_t performance : 4;  ///< [ 0: 3] : RW 0~e, 值越小，发送性能越低
#endif
} TxPerformance;

typedef struct
{
    #ifndef __BIG_ENDIAN__
    uint32_t buf_num : 12; ///< [ 0:11] : RW buf number, max 4096
    uint32_t num_of_128B : 7;  ///< [12:18] : RW 小于8kbytes部分，128bytes的个数 8k=64 16k=64 8193=1 0=0(不需要做dma)
    uint32_t flag_2056B : 1;  ///< [   19] : RW 2056bytes
    uint32_t flag_8192B : 1;  ///< [   20] : RW 8kbytes
    uint32_t message_num : 10; ///< [21:30] : RW channel message number
    uint32_t eop_flag : 1;  ///< [   31] : RW eop flag 0 - not eop 1 - eop
    #else
    uint32_t eop_flag : 1;  ///< [   31] : RW eop flag 0 - not eop 1 - eop
    uint32_t message_num : 10; ///< [21:30] : RW channel message number
    uint32_t flag_8192B : 1;  ///< [   20] : RW 8kbytes
    uint32_t flag_2056B : 1;  ///< [   19] : RW 2056bytes
    uint32_t num_of_128B : 7;  ///< [12:18] : RW 小于8kbytes部分，128bytes的个数 8k=64 16k=64 8193=1 0=0(不需要做dma)
    uint32_t buf_num : 12; ///< [ 0:11] : RW buf number, max 4096
    #endif
} BdDesc;

typedef struct
{
    #ifndef __BIG_ENDIAN__
    uint32_t buf_num : 12; ///< [11: 0]: buf number max 4096
    uint32_t num_of_128B : 7;  ///< [18:12]: 对小于等于8k部分128bytes个数,8kbytes=64;16kbytes=64;8193bytes=1
    uint32_t flag_2056B : 1;  ///< [   19]: 是否大于2056B
    uint32_t flag_8192B : 1;  ///< [   20]: 是否大于8192B
    uint32_t tr : 1;  ///< [   21]: T/R*
    uint32_t nt2nt : 1;  ///< [   22]: NT2NT
    uint32_t suppress_status : 1;  ///< [   23]: Suppress Status
    uint32_t retry : 2;  ///< [25:24]: 重试次数
    uint32_t reserved : 6;  ///< [26:31]: reserved
    #else
    uint32_t reserved : 6;  ///< [26:31]: reserved
    uint32_t retry : 2;  ///< [25:24]: 重试次数
    uint32_t suppress_status : 1;  ///< [   23]: Suppress Status
    uint32_t nt2nt : 1;  ///< [   22]: NT2NT
    uint32_t tr : 1;  ///< [   21]: T/R*
    uint32_t flag_8192B : 1;  ///< [   20]: 是否大于8192B
    uint32_t flag_2056B : 1;  ///< [   19]: 是否大于2056B
    uint32_t num_of_128B : 7;  ///< [18:12]: 对小于等于8k部分128bytes个数,8kbytes=64;16kbytes=64;8193bytes=1
    uint32_t buf_num : 12; ///< [11: 0]: buf number max 4096
    #endif
} FpgaChannelMsg;

#define Reg_WriteIndirectAddrStruct32(reg, addr, direct, st) \
    Reg_WriteIndirectAddr32(reg, addr, direct, CastStruct2UInt32(st))


uint32_t Reg_GetFpgaVer(const FpgaRegBaseAddr *addr);
void     Reg_SetPortFcId(const FpgaRegBaseAddr *addr, const uint32_t fcid);
uint32_t Reg_GetPortFcid(const FpgaRegBaseAddr *addr);
void     Reg_SetPortCreditNum(const FpgaRegBaseAddr *reg, const uint32_t credit);
uint32_t Reg_GetPortCreditNum(const FpgaRegBaseAddr *reg);
void     Reg_SetPortSpeed(const FpgaRegBaseAddr *reg, const uint32_t speed);
uint32_t Reg_GetPortSpeed(const FpgaRegBaseAddr *reg);
void     Reg_SetPortDidFilterEnab(const FpgaRegBaseAddr *reg, const uint32_t did_filter_enab);
uint32_t Reg_GetPortDidFilterEnab(const FpgaRegBaseAddr *reg);
void     Reg_SetPortEnab(const FpgaRegBaseAddr *reg, const uint32_t enab);
uint32_t Reg_GetPortEnab(const FpgaRegBaseAddr *reg);
void     Reg_ResetFpga(const FpgaRegBaseAddr *reg, const uint32_t macForceOffLine);
void     Reg_ClearIndirectRegister(const FpgaRegBaseAddr *reg);
void     Reg_SetPortRedundantEnab(const FpgaRegBaseAddr *reg, const uint32_t redundancy_enab);
uint32_t Reg_GetPortRedundantEnab(const FpgaRegBaseAddr *reg);
void     Reg_SetPortEdtov(const FpgaRegBaseAddr *reg, const uint32_t edtov);
void     Reg_SetRtcControlRtcBase(const FpgaRegBaseAddr *reg, const uint32_t rtc_base);
void     Reg_SetNtTimeOut(const FpgaRegBaseAddr *reg, const uint32_t value);
uint32_t Reg_GetNtTimeOut(const FpgaRegBaseAddr *reg, const uint32_t value);
void     Reg_SetNt2NtTimeOut(const FpgaRegBaseAddr *reg, const uint32_t value);
uint32_t Reg_GetNt2NtTimeOut(const FpgaRegBaseAddr *reg, const uint32_t value);
void     Reg_SetRatovTimeout(const FpgaRegBaseAddr *reg, const uint32_t value);
uint32_t Reg_GetRatovTimeout(const FpgaRegBaseAddr *reg);
void     Reg_WriteIndirectAddr32(const FpgaRegBaseAddr *reg, const uint32_t addr, const uint32_t direct, const uint32_t val);
uint32_t Reg_ReadIndirectAddr32(const FpgaRegBaseAddr *reg, const uint32_t addr, const uint32_t direct);
void     Reg_NcSendCmd(const FpgaRegBaseAddr *reg, const Fh228Priority priority, const int msgNo);
void     Reg_PushChannelDmaBuf2Fpga(const FpgaRegBaseAddr* reg, ChnProp * chn, const size_t size, const int nodeIndex);

void Reg_PushDmaBuf2Fpga(
    const FpgaRegBaseAddr *reg,
    const int dmaIndex,
    const int dmaBytes,
    const int msgNo,
    const int eop,
    const Fh228Priority priority
);

#endif
