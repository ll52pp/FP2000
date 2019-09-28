#include "period_msg.h"
#include "fpga_reg_addr.h"
#include "utils.h"
#include "struct.h"
#include "reg.h"

#define MAX_TIME_INTERVAL_COUNT     128
#define LOG_TAG                     "period_msg"

// 内部函数在此声明...
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
    CsLinkedHashMapContext timeIntervalMap; ///< 时间间隔表, 键值对是(int timeInterval, int index)
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
    // FPGA只能存储128个时间间隔的值. 所以这里做以下几件事
    // 1. 将所有时间间隔, 拿出来, 存到一个临时数组中
    // 2. 将数据排序后去重.
    // 3. 判断去重后的数量, 是否大于128
    // 4. 将去重后的数据, 存到一张哈希表中, 便于以后根据时间间隔, 可以很快查找到它所对应的下标
    // 5. 将去重后的数据, 注册到FPGA中

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
