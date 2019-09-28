#include "config_mgr.h"
#include "fpga_reg_addr.h"
#include "utils.h"
#include "send_que.h"
#include "reg.h"

#define LOG_TAG "config"
#define HASH_CODE_INVALID 0xFEFEFEFE

typedef union
{
    FcDidRCtl                   did;
    FcSidCsCtlPriority          sid;
    FcFCtlType                  fctl;
    FcAc1553CommandSeqWord6     cmdHdr1;
    unsigned int                subAddr;
    unsigned int                dataBytes;
    FcAe1553CommandSeqWord10    otherDid;
    unsigned int                otherSubAddr;
    FcSeqCntDfCtlSeqId          seq;
    FcRxIdOxId                  oxidRxid;
    FcAc1553CommandSeqWord6     status1;
    unsigned int                status2;

    unsigned int rawValue; ///< 这个字段用来将这个union清0
} CfgTxHdrVal;

// 内部函数在此声明
static void _Cfg_InitHashMap(CfgMgrCtx* ctx, const Fh228ChannelProperty properties[], int count);
static void _Cfg_UninitHashMap(CfgMgrCtx* ctx);
static void _Cfg_InitPingPangBuf(CfgMgrCtx* ctx, SendQueContext* que);
static void _Cfg_InitFpgaTxHdr(CfgMgrCtx* ctx);

static void Cfg_SetFpgaTxHdr(FpgaRegBaseAddr* reg, const ChnProp* chn);
static bool _Cfg_SetConditionHashAndMapImpl(const FpgaRegBaseAddr* reg, const uint32_t subAddr, const int index);
static void _Cfg_ClearConditionHashReg(FpgaRegBaseAddr *reg);

void CfgMgr_Init(CfgMgrCtx* ctx, FpgaRegBaseAddr* reg)
{
    ctx->reg = reg;
    Cs_LinkedHashMapInit(&ctx->chnMap, FH228_MAX_CHANNEL_COUNT);
}

void CfgMgr_Uninit(CfgMgrCtx* ctx)
{
    _Cfg_UninitHashMap(ctx);
    ctx->reg = NULL;
}

void CfgMgr_SetRole(CfgMgrCtx* ctx, const Fh228Role role)
{
    ctx->role = role;
}

Fh228Role CfgMgr_GetRole(CfgMgrCtx* ctx)
{
    return ctx->role;
}

bool CfgMgr_IsNtRole(CfgMgrCtx* ctx)
{
    return (FH228_ROLE_NT == CfgMgr_GetRole(ctx));
}

bool CfgMgr_IsNcRole(CfgMgrCtx* ctx)
{
    return (FH228_ROLE_NC == CfgMgr_GetRole(ctx));
}

void CfgMgr_SetChnCfg(CfgMgrCtx* ctx, const Fh228ChannelProperty properties[], int count, SendQueContext* periodQue)
{
    _Cfg_InitHashMap(ctx, properties, count);
    _Cfg_InitPingPangBuf(ctx, periodQue);
    _Cfg_InitFpgaTxHdr(ctx);
}

static bool _Cfg_SetConditionHashAndMapImpl(const FpgaRegBaseAddr* reg, const uint32_t subAddr, const int index)
{
    const int direct = 0;
    int conflict = 0;
    unsigned char hashcode = Utils_FpgaHashCode(0, subAddr);

    for (conflict = 0; conflict < 4; ++conflict)
    {
        uint32_t value = Reg_ReadIndirectAddr32(reg, FPGA_REG_ADDR_CONDITION_HASH + conflict * 256 + hashcode * 2, direct);
        if (value == HASH_CODE_INVALID)
        {
            Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_CONDITION_HASH + conflict * 256 + hashcode * 2, direct, subAddr);
            Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_CONDITION_HASH + conflict * 256 + hashcode * 2 + 1, direct, 0);
            Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_CONDITION_MAP + conflict * 128 + hashcode, direct, index);

            elog_d("hash&map", "hash[addr=0x%08x sa=0x%08x], map[addr=0x%08x, id=%d], hashcode=%u, conflict=%d",
                   FPGA_REG_ADDR_CONDITION_HASH + conflict * 256 + hashcode * 2,
                   subAddr,
                   FPGA_REG_ADDR_CONDITION_MAP + conflict * 128 + hashcode,
                   index,
                   hashcode,
                   conflict
            );

            break;
        }
    }

    if (conflict >= 4)
    {
        elog_e(LOG_TAG, "hashcode conflict, sa=%u, hashcode=0x%02x", subAddr, hashcode);
        return false;
    }

    return true;
}

bool CfgMgr_SetConditionHashAndMap(CfgMgrCtx* ctx)
{
    CsLinkedHashMapBucket* bucket = Cs_LinkedHashMapBegin(&ctx->chnMap);
    _Cfg_ClearConditionHashReg(ctx->reg); // 先清空
    for (; bucket != NULL; bucket = Cs_LinkedHashMapNext(bucket))
    {
        ChnProp* chn = (ChnProp*)bucket->val;
        if (false == _Cfg_SetConditionHashAndMapImpl(ctx->reg, chn->s_subAddr, chn->msgNo))
        {
            return false;
        }
    }

    return true;
}

ChnProp* CfgMgr_GetChnProp(CfgMgrCtx* ctx, const uint32_t subAddr)
{
    return (ChnProp*)Cs_LinkedHashMapGet(&ctx->chnMap, (void*)subAddr);
}

void Cfg_SetFpgaTxHdr(FpgaRegBaseAddr* reg, const ChnProp* chn)
{
    const uint32_t addr = FPGA_REG_ADDR_TX_BUF_HDR + chn->msgNo * 16;
    const uint32_t direct = 1;
    CfgTxHdrVal st;

    ZeroStruct(st);
    st.did.did = chn->s_did;
    st.did.r_ctl = 0x06;
    Reg_WriteIndirectAddrStruct32(reg, addr + 0, direct, st);

    ZeroStruct(st);
    st.sid.sid = chn->s_sid;
    if (chn->s_priority > 0)
    {
        st.sid.cs_ctl_priority = 1;
    }
    Reg_WriteIndirectAddrStruct32(reg, addr + 1, direct, st);

    ZeroStruct(st);
    st.fctl.f_ctl = (0x002A0000 >> 2);
    st.fctl.type = 0x48;
    Reg_WriteIndirectAddrStruct32(reg, addr + 2, direct, st);

    ZeroStruct(st);
    st.cmdHdr1.multicast = (chn->s_did >= 0xFFFB00) && (chn->s_did <= 0xFFFBFE) ? 1 : 0;
    st.cmdHdr1.tr = chn->s_tr;
    st.cmdHdr1.nt_to_nt_transfer = chn->S_chn.nt2nt;
    st.cmdHdr1.suppress_status = chn->s_suppressStatus;
    Reg_WriteIndirectAddrStruct32(reg, addr + 3, direct, st);

    ZeroStruct(st);
    st.subAddr = chn->s_subAddr;
    Reg_WriteIndirectAddrStruct32(reg, addr + 4, direct, st);

    ZeroStruct(st);
    st.dataBytes = chn->s_dataByteCount;
    Reg_WriteIndirectAddrStruct32(reg, addr + 5, direct, st);

    ZeroStruct(st);
    st.otherDid.multicast_address_other_port_id = chn->s_otherDid;
    Reg_WriteIndirectAddrStruct32(reg, addr + 6, direct, st);

    ZeroStruct(st);
    st.otherSubAddr = chn->s_otherSubAddr;
    Reg_WriteIndirectAddrStruct32(reg, addr + 7, direct, st);

    ZeroStruct(st);
    Reg_WriteIndirectAddrStruct32(reg, addr + 8, direct, st);

    ZeroStruct(st);
    st.oxidRxid.ox_id = 0;
    st.oxidRxid.rx_id = 0xffff;
    Reg_WriteIndirectAddrStruct32(reg, addr + 9, direct, st);

    ZeroStruct(st);
    Reg_WriteIndirectAddrStruct32(reg, addr + 10, direct, st);

    ZeroStruct(st);
    Reg_WriteIndirectAddrStruct32(reg, addr + 11, direct, st);
}

// void Cfg_SetFpgaChannel(FpgaRegBaseAddr* reg, const ChnProp* chn)
// {
//     FpgaChannelMsg msg;
//     ZeroStruct(msg);
// 
//     msg.suppress_status = chn->s_suppressStatus;
//     msg.retry = chn->s_retryNum;
//     msg.tr = chn->s_tr;
// 
//     Reg_WriteIndirectAddrStruct32(reg, FPGA_REG_ADDR_CHANNEL_MSG_BUF + chn->msgNo, 1, msg);
// }

static void _Cfg_ClearConditionHashReg(FpgaRegBaseAddr *reg)
{
    int i = 0;
    for (; i < 1024; ++i)
    {
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_CONDITION_HASH + i, 0, HASH_CODE_INVALID);
    }
}

static void _Cfg_InitPingPangBuf(CfgMgrCtx* ctx, SendQueContext* que)
{
    // 周期消息用的是乒乓buf
    // 所以遍历整个周期消息的发送链表, 两两一对, 给每个消息通道的乒乓buf赋值初始化
    ChnProp* chn = NULL;
    SendQueNode* ping         = NULL;
    SendQueNode* pong         = NULL;
    int nodeSize              = SendQue_GetNodeSize(que);

    CsLinkedHashMapBucket* iter = Cs_LinkedHashMapBegin(&ctx->chnMap);
    for (; iter != NULL; iter = Cs_LinkedHashMapNext(iter))
    {
        ping = SendQue_PeekFront(que); // 取第一个节点
        if (NULL == ping)
        {
            break;
        }

        SendQue_Move2NextNode(que);
        pong = SendQue_PeekFront(que); // 取第二个节点
        if (NULL == pong)
        {
            break;
        }

        // 给通道的乒乓buf初始化
        chn = (ChnProp*)iter->val;
        Cs_PingPongBufInit(&chn->periodMsg, (char*)ping, (char*)pong, nodeSize);
        SendQue_Move2NextNode(que); // 移动到第三个节点
    }
}

static void _Cfg_InitHashMap(CfgMgrCtx* ctx, const Fh228ChannelProperty userProps[], int count)
{
    int i = 0;
    for (i = 0; i < count; i++)
    {
        // 申请一块内存, 用于存储一个通道的所有属性
        ChnProp* chn = (ChnProp*)malloc(sizeof(ChnProp));

        // 初始化通道属性的各个字段
        chn->msgNo = i;
        memcpy(&chn->S_chn, &userProps[i], sizeof(Fh228ChannelProperty));
        RecvQue_Init(&chn->recvQue, 0, 0, 0);
        Cs_PingPongBufInit(&chn->periodMsg, NULL, NULL, 0);

        // 将初始化好的通道属性存入哈希表
        Cs_LinkedHashMapInsert(&ctx->chnMap, (void*)chn->s_subAddr, (void*)chn);
    }
}

static void _Cfg_UninitHashMap(CfgMgrCtx* ctx)
{
    CsLinkedHashMapBucket* bucket = Cs_LinkedHashMapBegin(&ctx->chnMap);
    for (; bucket != NULL; bucket = Cs_LinkedHashMapNext(bucket))
    {
        ChnProp* chn = (ChnProp*)bucket->val;
        RecvQue_Uninit(&chn->recvQue);
        Cs_PingPongBufUninit(&chn->periodMsg);
        free(chn);
    }
    Cs_LinkedHashMapUninit(&ctx->chnMap);
}

static void _Cfg_InitFpgaTxHdr(CfgMgrCtx* ctx)
{
    CsLinkedHashMapBucket* bucket = Cs_LinkedHashMapBegin(&ctx->chnMap);
    for (; bucket != NULL; bucket = Cs_LinkedHashMapNext(bucket))
    {
        ChnProp* chn = (ChnProp*)bucket->val;
        Cfg_SetFpgaTxHdr(ctx->reg, chn);
    }
}
