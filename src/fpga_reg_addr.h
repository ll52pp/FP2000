#ifndef FPGA_REG_ADDR_H
#define FPGA_REG_ADDR_H

#include "common_header.h"

#define FPGA_REG_ADDR_RX_BUF_SIZE_LOW            0x1701 ///< 低优先级RX buf的大小
#define FPGA_REG_ADDR_RX_BUF_SIZE_MID            0x1702 ///< 中优先级RX buf的大小
#define FPGA_REG_ADDR_RX_BUF_SIZE_HIGH           0x1703 ///< 高优先级RX buf的大小
#define FPGA_REG_ADDR_RX_BUF_LA_LOW              0x0000 ///< 低优先级RX buf的逻辑地址, 范围[0x0000, 0x03ff] 共1024个
#define FPGA_REG_ADDR_RX_BUF_LA_MID              0x0400 ///< 中优先级RX buf的逻辑地址, 范围[0x0400, 0x07ff] 共1024个
#define FPGA_REG_ADDR_RX_BUF_LA_HIGH             0x2800 ///< 高优先级RX buf的逻辑地址, 范围[0x2800, 0x2bff] 共1024个
#define FPGA_REG_ADDR_TX_BUF_LA                  0x4000 ///< TX buf的逻辑地址, 范围[0x4000, 0x4ffff] 共4k个
#define FPGA_REG_ADDR_TX_BUF_HDR                 0x0000 ///< TX buf的HDR, 范围[0, 3fff] 共16K个
#define FPGA_REG_ADDR_CHANNEL_MSG_BUF            0x5800 ///< 通道消息buf, 范围[0x5800, 0x5bff]共1024个
#define FPGA_REG_ADDR_CONDITION_HASH             0x1000 ///< 条件hash, 范围[0x1000, 0x13ff] 共1024个
#define FPGA_REG_ADDR_CONDITION_MAP              0x1800 ///< 条件map, 范围[0x1800, 0x19ff] 共512个
#define FPGA_REG_ADDR_PERIODIC_MSG_TIME_INTERVAL 0x5c00 ///< 周期消息时间间隔, 范围[0x5c00, 0x5c7f] 共128个
#define FPGA_REG_ADDR_PERIODIC_MSG_DESCRIPTOR    0x5000 ///< 周期指令描述, 范围[0x5000, 0x57fff] 共2048个

/* 寄存器定义*/
typedef struct
{
    uint32_t fpgaVer;
    /* offset : 0x00
    [ 0:31] : RO fpga version -- fpga version*/

    uint32_t portCtrl;
    /* offset : 0x04
    [   31] : RW reserved --
    [   30] : RW did filter enable -- 0-enable 1-disable default '1'
    [28:29] : RW speed -- 00 auto sense 01 1Gbps 10 2Gbps 11 4Gbps
    [24:27] : RW credit -- set 0000'b～1111'b=>1～16 number of credit default '1'
    [ 0:23] : RW sid -- self id default '0'*/

    uint32_t credit0;
    /* offset : 0x08
    [30:31] : RW valid number of buffers:[1, 3]
    [20:29] : RW 3rd available buffer number
    [10:19] : RW 2nd available buffer number
    [ 0: 9] : RW 1st available buffer number*/

    uint32_t portEnab;
    /* offset : 0x0C
    [16:31] : RW E_d_tov value, unit 1ms
    [15   ] : RW Redundant enable -- 0-disable 1-enab
    [13:14] : RW tx raw 使用的优先级 0-发送低优先级 1-    2-发送高优先级
    [ 3:12] : RW reserved
    [ 2   ] : RW fpga reset/unload config 1-reset/unload config, fpga self clear
    [ 1   ] : RW Mac force off -- 0-mac on 1-mac off
    [ 0   ] : RW port enab -- port enable 0-disable 1-enable*/

    uint32_t bdDest0;
    /* offset : 0x10
    [   31] : RW eop flag 0-not eop 1-eop
    [21:30] : RW channel message number
    [   20] : RW >8kbytes
    [   19] : RW >2056bytes
    [12:18] : RW 小于8kbytes部分，128bytes的个数 8k=64 16k=64 8193=1 0=0(不需要做dma)
    [ 0:11] : RW buf number, max 4096*/

    uint32_t rtcCtrl;
    /* offset : 0x14
    [21:31] : RW reserve
    [20   ] : RW rtc write enable -- 0-disable 1-enable
    [19   ] : RW Hba card hba global sync enable -- 0-disable 1-enable
    [18   ] : RW rtc base -- 0-5x  division,used in 1G/2G mode
    1-10x division,used in 2G/4G mode
    [17   ] : RW port1 sync enable -- 0-disable 1-enable
    [16   ] : RW port0 sync enable -- 0-disable 1-enable
    [ 0:15] : RW event period unit is ms
    Note: The actual event period equals the programmed value plus one.*/

    uint32_t portState;
    /* offset : 0x18
    [20:31] : RO fpga temperature
    [ 8:19] : RO fpga voltage
    [ 3: 7] : RW reserve  -- reserve
    [ 2   ] : RO reset done
    [ 1   ] : RO port1 status -- 0-loss of sync 1-sync
    [ 0   ] : RO port0 status -- 0-loss of sync 1-sync*/

    uint32_t indirectCtrl;
    /* offset : 0x1C
    [   31] : RW direct flag  -- 0-host read 1-host write
    [   30] : RW reserve
    [   29] : RW direct       -- 0-RX 1-TX
    [16:28] : RW reserve
    [ 0:15] : RW address*/

    uint32_t indirectData;
    /* offset : 0x20
    [ 0:31] : RW data*/

    uint32_t credit1;
    /* offset : 0x24
    [30:31] : RW valid number of buffers:[1, 3]
    [20:29] : RW 3rd available buffer number
    [10:19] : RW 2nd available buffer number
    [ 0: 9] : RW 1st available buffer number*/

    uint32_t rtcTimeWriteH;
    /* offset : 0x28
    [10:31] : RW reserve
    [ 0: 9] : RW rtc high bit [32:41]*/

    uint32_t rtcTimeWriteL;
    /* offset : 0x2C
    [ 0:31] : RW rtc low bit [ 0:31]*/

    uint32_t rtcTimeReadH;
    /* offset : 0x30
    [10:31] : RW reserve
    [ 0: 9] : RW rtc high bit [32:41]*/

    uint32_t rtcTimeReadL;
    /* offset : 0x34
    [ 0:31] : RW rtc low bit [ 0:31]*/

    uint32_t statisticsCtrl;
    /* offset : 0x38
    [31   ] : RW start statistic   -- RW set 1 start statistic, self clear
    [ 6:30] : RW reserve
    [ 0: 5] : RW type of statistic --
    0-cur timestamp low [ 0:31]
    1-recv word num
    2-recv idle num
    3-recv frame num
    4-recv iu num
    5-reserved
    6-reserved
    7-reserved

    8-cur timestamp high [32:41]
    9-send word num
    10-send idle num
    11-send frame num
    12-send iu num
    13-reserved
    14-reserved
    15-reserved
    */

    uint32_t statisticsData;
    /* offset : 0x3C
    [ 0:31] : RO data of statistics*/

    uint32_t ntTimeout;
    /* offset : 0x40
    [24:31] : RW reserve
    [ 0:23] : RW timeout, unit 1ms nt2nt=0*/

    uint32_t nt2ntTimeout;
    /* offset : 0x44
    [24:31] : RW reserve
    [ 0:23] : RW timeout, unit 1ms nt2nt=1*/

    uint32_t flashCtrl;
    /* offset : 0x48
    [31   ] : RW wr_flash -- write flash enable 1-enable 0-disable
    [30   ] : RW wr_flash_debug
    [29   ] : RW rd_flash -- read flash enable 1-enable 0-disable
    [28   ] : RW rd_flash_debug
    [24:27] : RW reserve
    [ 0:23] : RW flash_addr*/

    uint32_t interruptCtrl;
    /* offset : 0x4C
    [31   ] : RW Global intr enable
    [30   ] : RW Link change intr enable
    [29   ] : RW 所有发送bd fifo空中断使能
    [28   ] : RW Tx IU intr enable
    [27   ] : RW 任何一个优先级无信用中断使能
    [26   ] : RW Rx IU intr enable
    [ 5:25] : RW reserved
    [ 4   ] : RW Link change intr pending
    [ 3   ] : RW 所有发送bd fifo空中断发生
    [ 2   ] : RW Tx IU intr pending
    [ 1   ] : RW 任何一个优先级无信用中断发生
    [ 0   ] : RW Rx IU intr pending*/

    uint32_t rtcDateWrite;
    /* offset : 0x50 RTC天数配置寄存器*/

    uint32_t rtcDateRead;
    /* offset : 0x54 RTC天数实时寄存器
    [31   ] : RW snap shot command 1-启动快照
    [16:30] : RO reserve
    [ 0:15] : RO real time day*/

    uint32_t bdDest1;
    /* offset : 0x58
    [   31] : RW eop flag 0-not eop 1-eop
    [21:30] : RW channel message number
    [   20] : RW >8kbytes
    [   19] : RW >2056bytes
    [12:18] : RW 小于8kbytes部分，128bytes的个数 8k=64 16k=64 8193=1 0=0(不需要做dma)
    [ 0:11] : RW buf number, max 4096*/

    uint32_t bdDest2;
    /* offset : 0x5C
    [   31] : RW eop flag 0-not eop 1-eop
    [21:30] : RW channel message number
    [   20] : RW >8kbytes
    [   19] : RW >2056bytes
    [12:18] : RW 小于8kbytes部分，128bytes的个数 8k=64 16k=64 8193=1 0=0(不需要做dma)
    [ 0:11] : RW buf number, max 4096*/

    uint32_t ratovTimeout;
    /* offset : 0x60
    [24:31] : RW reserved
    [ 0:23] : RW r_a_tov timeout unit 1ms*/

    uint32_t switchFcId;
    /* offset : 0x64
    [24:31] : RW reserved
    [ 0:23] : RW switch fcid*/

    uint32_t sfpRxDbg;
    /* offset : 0x68
    [24:31] : RW DAT_IN_BYTE
    [20:23] : RW SOP_IN_CNT
    [16:19] : RW EOP_IN_CNT
    [ 8:15] : RW DAT_OUT_BYTE
    [ 4: 7] : RW SOP_OUT_CNT
    [ 0: 3] : RW EOP_OUT_CNT
    */

    uint32_t txPerformance;
    /* offset : 0x6C
    [ 4:31] : RW reserved
    [ 0: 3] : RW 0~e, 值越小，发送性能越低*/

    uint32_t credit2;
    /* offset : 0x70
    [30:31] : RW valid number of buffers:[1, 3]
    [20:29] : RW 3rd available buffer number
    [10:19] : RW 2nd available buffer number
    [ 0: 9] : RW 1st available buffer number*/
} FpgaRegBaseAddr;

#endif
