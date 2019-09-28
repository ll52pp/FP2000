#include "period_msg.h"
#include "fpga_reg_addr.h"
#include "utils.h"
#include "struct.h"
#include "reg.h"

#define MAX_TIME_INTERVAL_COUNT     128
#define LOG_TAG                     "period_msg"

// �ڲ������ڴ�����...
static void PeriodMsg_SetTimeInterval2Fpga(const FpgaRegBaseAddr *reg, const int index, const int milliSeconds);
static bool PeriodMsg_SetTimeInterval(
    const FpgaRegBaseAddr* reg,
    const Fh228PeriodicMsgCmd* cmds,
    const int count,
    CsLinkedHashMapContext* timeIntervalMap
);
static void PeriodMsg_SetCmd2Fpga(
    const FpgaRegBaseAddr* reg,
    const Fh228PeriodicMsgCmd *cmds,
    const int count,
    CsLinkedHashMapContext* timeIntervalMap
);

bool PeriodMsg_SetCmd(const FpgaRegBaseAddr* reg, const Fh228PeriodicMsgCmd* cmds, const int count)
{
    bool ret = false;
    CsLinkedHashMapContext timeIntervalMap; ///< ʱ������, ��ֵ����(int timeInterval, int index)
    Cs_LinkedHashMapInit(&timeIntervalMap, count);

    ret = PeriodMsg_SetTimeInterval(reg, cmds, count, &timeIntervalMap);
    if (ret)
    {
        PeriodMsg_SetCmd2Fpga(reg, cmds, count, &timeIntervalMap);
        ret = true;
    }

    Cs_LinkedHashMapUninit(&timeIntervalMap);
    return ret;
}

bool PeriodMsg_SetTimeInterval(const FpgaRegBaseAddr* reg, const Fh228PeriodicMsgCmd* cmds, const int count, CsLinkedHashMapContext* timeIntervalMap)
{
    // FPGAֻ�ܴ洢128��ʱ������ֵ. �������������¼�����
    // 1. ������ʱ����, �ó���, �浽һ����ʱ������
    // 2. �����������ȥ��.
    // 3. �ж�ȥ�غ������, �Ƿ����128
    // 4. ��ȥ�غ������, �浽һ�Ź�ϣ����, �����Ժ����ʱ����, ���Ժܿ���ҵ�������Ӧ���±�
    // 5. ��ȥ�غ������, ע�ᵽFPGA��

    bool ret = false;
    int uniqueCnt = 0;
    int *arr = malloc(sizeof(int) * count);
    int i = 0;
    for (i = 0; i < count; i++)
    {
        arr[i] = cmds[i].timeInterval;
    }

    Utils_BubleSortInt(arr, count);
    uniqueCnt = Utils_UniqueInt(arr, count);

    if (uniqueCnt >= MAX_TIME_INTERVAL_COUNT)
    {
        goto Exit0;
    }

    for (i = 0; i < uniqueCnt; i++)
    {
        Cs_LinkedHashMapInsert(timeIntervalMap, (void*)arr[i], (void*)i);
        PeriodMsg_SetTimeInterval2Fpga(reg, i, arr[i]);
    }

    ret = true;

Exit0:
    free(arr);
    return ret;
}

void PeriodMsg_SetTimeInterval2Fpga(const FpgaRegBaseAddr *reg, const int index, const int milliSeconds)
{
    if (index <= 127)
    {
        Reg_WriteIndirectAddr32(reg, FPGA_REG_ADDR_PERIODIC_MSG_TIME_INTERVAL + index, 1, milliSeconds);
    }
    else
    {
        elog_e(LOG_TAG, "index not found, index = %d", index);
    }
}

void PeriodMsg_SetFpgaEnable(const FpgaRegBaseAddr *reg, const int msgNo, const bool enable)
{
    Fc1553R0PeriodInst val;
    ZeroStruct(val);
    val.enab = enable;
    Reg_WriteIndirectAddrStruct32(reg, FPGA_REG_ADDR_PERIODIC_MSG_DESCRIPTOR + msgNo, 1, val);
}

void PeriodMsg_SetCmd2Fpga(
    const FpgaRegBaseAddr* reg,
    const Fh228PeriodicMsgCmd *cmds,
    const int count,
    CsLinkedHashMapContext* timeIntervalMap
)
{
    int i = 0;
    Fc1553R0PeriodInst val;
    ZeroStruct(val);

    for (i = 0; i < count; ++i)
    {
        val.enab = cmds[i].isEnabled;
        val.inst_type = cmds[i].type;
        val.content = cmds[i].content;
        val.jmp_index = cmds[i].jumpIndex;
        val.interval_index = (unsigned int)Cs_LinkedHashMapGet(timeIntervalMap, (void*)cmds[i].timeInterval);
        Reg_WriteIndirectAddrStruct32(reg, FPGA_REG_ADDR_PERIODIC_MSG_DESCRIPTOR + i, 1, val);
    }
}
