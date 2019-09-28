#ifndef CONFIGER_H
#define CONFIGER_H

#include "common_header.h"
#include "fh228/fh228_def.h"
#include "recv_que.h"
#include "send_que.h"

/**
 * @brief �豸��������Ϣ
 */
typedef struct
{
    Fh228Role               role;   ///< ��ɫ, NT����NC
    CsLinkedHashMapContext  chnMap; ///< ��Ϣͨ��������, ����֧�ֵ�1024����Ϣͨ��, ��ֵ��(int sa, CfgChannelPropertyWrapper*)
    FpgaRegBaseAddr*        reg;    ///< �豸�ļĴ�������ַ
} CfgMgrCtx;

/**
 * ������Ϣͨ����������Ϣ
 */
typedef struct
{
    /**
     * msgNo�ֶ�, ��FPGAʹ��, ��Χ[0,1023), ��ΪFPGA��֧��1024����Ϣ
     * ���ӵ�ַ��Χ��[0,0xffffffff), ����Ҫ����ֶ�����ʶ��ǰ�ӵ�ַ��Ӧ��msgNo�Ƕ���
     */
    int                  msgNo;
    RecvQueContext       recvQue;
    CsPingPongBufContext periodMsg;
    Fh228ChannelProperty S_chn;

#define s_subAddr           S_chn.subAddr
#define s_priority          S_chn.priority
#define s_sid               S_chn.sid
#define s_did               S_chn.did
#define s_otherDid          S_chn.otherDid
#define s_otherSubAddr      S_chn.otherSubAddr
#define s_suppressStatus    S_chn.suppressStatus
#define s_retryNum          S_chn.retryNum
#define s_nt2nt             S_chn.nt2nt
#define s_tr                S_chn.tr
#define s_dataByteCount     S_chn.dataByteCount
} ChnProp;

void        CfgMgr_Init(CfgMgrCtx* ctx, FpgaRegBaseAddr* reg);
void        CfgMgr_Uninit(CfgMgrCtx* ctx);
void        CfgMgr_SetRole(CfgMgrCtx* ctx, const Fh228Role role);
Fh228Role   CfgMgr_GetRole(CfgMgrCtx* ctx);
bool        CfgMgr_IsNtRole(CfgMgrCtx* ctx);
bool        CfgMgr_IsNcRole(CfgMgrCtx* ctx);
void        CfgMgr_SetChnCfg(CfgMgrCtx* ctx, const Fh228ChannelProperty properties[], int count, SendQueContext* periodQue);
bool        CfgMgr_SetConditionHashAndMap(CfgMgrCtx* ctx);
ChnProp*    CfgMgr_GetChnProp(CfgMgrCtx* ctx, const uint32_t subAddr);

#define CfgMgr_IsChnTx(pChannel) (FH228_DATA_DIRECT_TX == (pChannel)->s_tr)
#define CfgMgr_IsChnRx(pChannel) (FH228_DATA_DIRECT_RX == (pChannel)->s_tr)

#endif
