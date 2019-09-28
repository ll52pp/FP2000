#include "reg.h"
#include "fpga_reg_addr.h"
#include "utils.h"

#define LOG_TAG "hw"

uint32_t Reg_GetFpgaVer(const FpgaRegBaseAddr *addr)
{
    return Ba_RegisterRead32(&addr->fpgaVer);
}

void Reg_SetPortFcId(const FpgaRegBaseAddr *addr, const uint32_t fcid)
{
    PortCfg v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&addr->portCtrl);
    v.sid = fcid;
    Ba_RegisterWrite32(&addr->portCtrl, CastStruct2UInt32(v));
}

uint32_t Reg_GetPortFcid(const FpgaRegBaseAddr *reg)
{
    PortCfg v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portCtrl);
    return v.sid;
}

void Reg_SetPortCreditNum(const FpgaRegBaseAddr *reg, const uint32_t credit)
{
    PortCfg v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portCtrl);
    assert(credit >= 1 && credit <= 16);
    v.credit = credit - 1;
    Ba_RegisterWrite32(&reg->portCtrl, CastStruct2UInt32(v));
}

uint32_t Reg_GetPortCreditNum(const FpgaRegBaseAddr *reg)
{
    PortCfg v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portCtrl);
    return v.credit + 1; /*0 - base*/
}

void Reg_SetPortSpeed(const FpgaRegBaseAddr *reg, const uint32_t speed)
{
    PortCfg v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portCtrl);
    v.speed = speed; /*0 自适应 1-1Gbps 2-2Gbps*/
    Ba_RegisterWrite32(&reg->portCtrl, CastStruct2UInt32(v));
}

uint32_t Reg_GetPortSpeed(const FpgaRegBaseAddr *reg)
{
    PortCfg v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portCtrl);
    return v.speed; /*0 自适应 1-1Gbps 2-2Gbps*/
}

/*DID过滤使能*/
void Reg_SetPortDidFilterEnab(const FpgaRegBaseAddr *reg, const uint32_t did_filter_enab)
{
    PortCfg v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portCtrl);
    v.did_filter_enab = did_filter_enab;
    Ba_RegisterWrite32(&reg->portCtrl, CastStruct2UInt32(v));
}

uint32_t Reg_GetPortDidFilterEnab(const FpgaRegBaseAddr *reg)
{
    PortCfg v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portCtrl);
    return v.did_filter_enab;
}

void Reg_SetPortEnab(const FpgaRegBaseAddr *reg, const uint32_t enab)
{
    PortEnab v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portEnab);
    v.reset = 0;
    v.mac_switch = 0;
    v.port_enab = enab; // 0-禁止 1-使能
    Ba_RegisterWrite32(&reg->portEnab, CastStruct2UInt32(v));
}

uint32_t Reg_GetPortEnab(const FpgaRegBaseAddr *reg)
{
    PortEnab v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portEnab);
    return v.port_enab;
}

void Reg_ResetFpga(const FpgaRegBaseAddr *reg, const uint32_t macForceOffLine)
{
    UINT32 i;
    PortEnab enab;
    PortState status;

    CastStruct2UInt32(enab) = Ba_RegisterRead32(&reg->portEnab);
    enab.reset = 1;
    enab.mac_switch = macForceOffLine;
    Ba_RegisterWrite32(&reg->portEnab, CastStruct2UInt32(enab));

    i = 0;
    while (i < 10)
    {
        Ba_TaskSleep(16);
        CastStruct2UInt32(status) = Ba_RegisterRead32(&reg->portState);
        if (status.reset_done == 1)
        {
            if (macForceOffLine == 1)
            {
                CastStruct2UInt32(enab) = Ba_RegisterRead32(&reg->portEnab);
                enab.mac_switch = 0;
                Ba_RegisterWrite32(&reg->portEnab, CastStruct2UInt32(enab));
            }
            break;
        }
        ++i;
    }
    if (i == 10)
    {
        elog_w(LOG_TAG, "wait fpga reset timeout");
    }
}

void Reg_ClearIndirectRegister(const FpgaRegBaseAddr *reg)
{
    INT32 i;
    for (i = 0; i < FH228_MAX_PERIODIC_MSG_TIME_INTERVAL_COUNT; ++i)
    {
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_PERIODIC_MSG_TIME_INTERVAL + i, 1, 0);
    }

    for (i = 0; i < FH228_MAX_CHANNEL_COUNT; ++i)
    {
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_CHANNEL_MSG_BUF + i, 1, 0);
    }

    for (i = 0; i < FH228_MAX_CHANNEL_COUNT; ++i)
    {
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + i * 16 + 0, 1, 0);
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + i * 16 + 1, 1, 0);
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + i * 16 + 2, 1, 0);
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + i * 16 + 3, 1, 0);
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + i * 16 + 4, 1, 0);
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + i * 16 + 5, 1, 0);
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + i * 16 + 6, 1, 0);
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + i * 16 + 7, 1, 0);
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + i * 16 + 8, 1, 0);
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + i * 16 + 9, 1, 0);
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + i * 16 + 10, 1, 0);
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + i * 16 + 11, 1, 0);
    }

    for (i = 0; i < FH228_MAX_PERIODIC_MSG_CMD_COUNT; ++i)
    {
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_PERIODIC_MSG_DESCRIPTOR + i, 1, 0);
    }
}

/*冗余使能*/
void Reg_SetPortRedundantEnab(const FpgaRegBaseAddr *reg, const uint32_t redundancy_enab)
{
    PortEnab v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portEnab);
    v.reset = 0;
    v.mac_switch = 0;
    v.redundancy_enab = redundancy_enab; // 0-禁止 1-使能
    Ba_RegisterWrite32(&reg->portEnab, CastStruct2UInt32(v));
}

uint32_t Reg_GetPortRedundantEnab(const FpgaRegBaseAddr *reg)
{
    PortEnab v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portEnab);
    return v.redundancy_enab;
}

void Reg_SetPortEdtov(const FpgaRegBaseAddr *reg, const uint32_t edtov)
{
    PortEnab v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portEnab);
    v.reset = 0;
    v.mac_switch = 0;
    v.edtov = edtov; // 单位是毫秒
    Ba_RegisterWrite32(&reg->portEnab, CastStruct2UInt32(v));
}

uint32_t HwGetPortEdtov(const FpgaRegBaseAddr *reg)
{
    PortEnab v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->portEnab);
    return v.edtov; // 单位是毫秒
}

void Reg_SetRtcControlRtcBase(const FpgaRegBaseAddr *reg, const uint32_t rtc_base)
{
    RtcCtrl v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->rtcCtrl);
    v.rtc_base = rtc_base;
    Ba_RegisterWrite32(&reg->rtcCtrl, CastStruct2UInt32(v));
}

/*NT TIMEOUT*/
void Reg_SetNtTimeOut(const FpgaRegBaseAddr *reg, const uint32_t value)
{
    NtTimeout v;
    ZeroStruct(v);
    v.time = value;
    Ba_RegisterWrite32(&reg->ntTimeout, CastStruct2UInt32(v));
}

uint32_t Reg_GetNtTimeOut(const FpgaRegBaseAddr *reg, const uint32_t value)
{
    NtTimeout v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->ntTimeout);
    return v.time;
}

/*NT2NT TIMEOUT*/
void Reg_SetNt2NtTimeOut(const FpgaRegBaseAddr *reg, const uint32_t value)
{
    Nt2NtTimeout v;
    ZeroStruct(v);
    v.time = value;
    Ba_RegisterWrite32(&reg->nt2ntTimeout, CastStruct2UInt32(v));
}

uint32_t Reg_GetNt2NtTimeOut(const FpgaRegBaseAddr *reg, const uint32_t value)
{
    Nt2NtTimeout v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->nt2ntTimeout);
    return v.time;
}

void Reg_SetRatovTimeout(const FpgaRegBaseAddr *reg, const uint32_t value)
{
    RATovTimeout v;
    ZeroStruct(v);
    v.r_a_tov_time_out = value;
    Ba_RegisterWrite32(&reg->ratovTimeout, CastStruct2UInt32(v));
}

uint32_t Reg_GetRatovTimeout(const FpgaRegBaseAddr *reg)
{
    RATovTimeout v;
    CastStruct2UInt32(v) = Ba_RegisterRead32(&reg->ratovTimeout);
    return v.r_a_tov_time_out;
}

void Reg_WriteIndirectAddr32(const FpgaRegBaseAddr *reg, const uint32_t addr, const uint32_t direct, const uint32_t val)
{
    IndirectCtrl ctl;
    ZeroStruct(ctl);
    ctl.address = addr;
    ctl.direct = direct;
    ctl.action = 1;
    Ba_RegisterWrite32(&reg->indirectData, val);
    Utils_DoEmptyLoop(500);
    Ba_RegisterWrite32(&reg->indirectCtrl, CastStruct2UInt32(ctl));
}

uint32_t Reg_ReadIndirectAddr32(const FpgaRegBaseAddr *reg, const uint32_t addr, const uint32_t direct)
{
    IndirectCtrl ctl;
    ZeroStruct(ctl);
    ctl.address = addr;
    ctl.direct = direct;
    ctl.action = 0;

    Ba_RegisterWrite32(&reg->indirectCtrl, CastStruct2UInt32(ctl));
    Utils_DoEmptyLoop(500);
    return Ba_RegisterRead32(&reg->indirectData);
}

void Reg_NcSendCmd(const FpgaRegBaseAddr *reg, const Fh228Priority priority, const int msgNo)
{
    BdDesc bd;
    ZeroStruct(bd);

    bd.message_num = msgNo;

    switch (priority)
    {
    case FH228_PRIORITY_LOW:
        Ba_RegisterWrite32Struct(&reg->bdDest0, bd);
        break;
    case FH228_PRIORITY_MID:
        Ba_RegisterWrite32Struct(&reg->bdDest1, bd);
        break;
    case FH228_PRIORITY_HIGH:
        Ba_RegisterWrite32Struct(&reg->bdDest2, bd);
        break;
    default:
        elog_e(LOG_TAG, "error priority, priority = %d", priority);
        break;
    }
}

void Reg_PushDmaBuf2Fpga(
    const FpgaRegBaseAddr *reg,
    const int dmaIndex,
    const int dmaBytes,
    const int msgNo,
    const int eop,
    const Fh228Priority priority)
{
    BdDesc bd;
    unsigned int little = 0;

    ZeroStruct(bd);

    bd.buf_num = dmaIndex;
    bd.message_num = msgNo;
    bd.eop_flag = eop;

    little = min(8192, dmaBytes);
    bd.num_of_128B = Cs_CalcByteAlignment(little, 128) / 128;
    bd.flag_2056B = dmaBytes > 2056 ? 1 : 0;
    bd.flag_8192B = dmaBytes > 8192 ? 1 : 0;

    switch (priority)
    {
    case FH228_PRIORITY_LOW:
        Ba_RegisterWrite32Struct(&reg->bdDest0, bd);
        break;
    case FH228_PRIORITY_MID:
        Ba_RegisterWrite32Struct(&reg->bdDest1, bd);
        break;
    case FH228_PRIORITY_HIGH:
        Ba_RegisterWrite32Struct(&reg->bdDest2, bd);
        break;
    default:
        elog_e(LOG_TAG, "error priority, priority = %d", priority);
        break;
    }

    //     elog_d(LOG_TAG, "push bd to fpga: bd=0x%08x,w0=0x%08x,bd.bufNum=%d,bd.msgNo=%d,bd.eop=%d,w0.sop=%d,w0.eop=%d,w0.len=%d",
    //         CastStruct2UInt32(bd), CastStruct2UInt32(w0),
    //         bd.buf_num, bd.message_num, bd.eop_flag,
    //         w0.sop, w0.eop, w0.length);
}

static int _CountOf128(int num)
{
    int count = 0;
    if (0 == num % 8192)
    {
        count = 8192 / 128;
    }
    else
    {
        count = Cs_CalcByteAlignment(num % 8192, 128) / 128;
    }
    return count;
}

void Reg_PushChannelDmaBuf2Fpga(const FpgaRegBaseAddr* reg, ChnProp * chn, const size_t size, const int nodeIndex)
{
	int msgNo = chn -> msgNo;
    FpgaChannelMsg msg;
    CastStruct2UInt32(msg) = Reg_ReadIndirectAddr32(reg, FPGA_REG_ADDR_CHANNEL_MSG_BUF + msgNo, 1);
    msg.buf_num = nodeIndex;
    msg.nt2nt = chn ->s_nt2nt;
    msg.tr = chn ->s_tr;
    msg.num_of_128B = _CountOf128(size);
    msg.flag_2056B = size > 2056 ? 1 : 0;
    msg.flag_8192B = size > 8192 ? 1 : 0;
    Reg_WriteIndirectAddrStruct32(reg, FPGA_REG_ADDR_CHANNEL_MSG_BUF + msgNo, 1, msg);
    Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_TX_BUF_HDR + msgNo * 16 + 5, 1, size);

    elog_d(LOG_TAG, "addr=0x5800+%d,val=0x%08x[buf_num=%d,num_of_128B=%d,flag_2056B=%d,flag_8192B=%d]",
        msgNo,
        CastStruct2UInt32(msg),
        msg.buf_num,
        msg.num_of_128B,
        msg.flag_2056B,
        msg.flag_8192B
    );

    elog_d(LOG_TAG, "addr=0x0000+%d,val=0x%08x[cmd_dataByteCount=%d]", msgNo * 16 + 5, size, size);
}
