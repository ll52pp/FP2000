#include "dev_mgr.h"

#define LOG_TAG "dev_mgr"

DevMgr* g_devMgr = NULL;

static DevMgr* DevMgr_GetInstance()
{
    if (!g_devMgr)
    {
        g_devMgr = malloc(sizeof(DevMgr));
        memset(g_devMgr, 0, sizeof(DevMgr));
        g_devMgr->isInit = false;
    }

    return g_devMgr;
}

bool DevMgr_Init()
{
    const int vid         = 0x10EE;
    const int did         = 0x7019;
    const int maxDevCnt   = 2;
    int i                 = 0;
    int realDevCnt        = 0;
    bool suc              = false;
    DevBaseInfo* baseInfo = NULL;
    DevMgr* inst = DevMgr_GetInstance();

    if (inst->isInit) // 若已经初始化过了, 就不要再初始化了
    {
        suc = true;
        goto Exit0;
    }

    baseInfo = (DevBaseInfo*)malloc(sizeof(DevBaseInfo) * maxDevCnt);

    // 根据VID, DID查找所有硬件设备
    realDevCnt = Utils_FindDev(vid, did, baseInfo, maxDevCnt);
    if (realDevCnt <= 0)
    {
        elog_e(LOG_TAG, "device not found, find dev cnt = %d, vid=0x%04x, did=0x%04x", realDevCnt, (uint32_t)vid, (uint32_t)did);
        goto Exit0;
    }
    else
    {
        elog_v(LOG_TAG, "succed to find dev, devCount=%d, vid=0x%04x, did=0x%04x", realDevCnt, (uint32_t)vid, (uint32_t)did);
    }

    inst->devList = (DevContext*)malloc(sizeof(DevContext) * realDevCnt);
    inst->devCnt = realDevCnt;
    for (i = 0; i < realDevCnt; i++)
    {
        DevContext* dc = &inst->devList[i];
        DevCtx_Init(dc);
        DevCtx_SetBusNo(dc, baseInfo[i].busNo);
        DevCtx_SetDevNo(dc, baseInfo[i].deviceNo);
        DevCtx_SetFuncNo(dc, baseInfo[i].funcNo);
        DevCtx_SetRegBaseAddr(dc, (FpgaRegBaseAddr*)baseInfo[i].membaseCsr);
        DevCtx_SetIrq(dc, baseInfo[i].irq);
    }

    suc = true;
    g_devMgr->isInit = true;

Exit0:
    if (baseInfo)
    {
        free(baseInfo);
    }
    return suc;
}

void DevMgr_Uninit()
{
    DevMgr* inst = DevMgr_GetInstance();
    if (inst->devList)
    {
        free(inst->devList);
        inst->devList = NULL;
    }
    inst->devCnt = 0;
}

DevContext* DevMgr_GetDevice(const int devId)
{
    DevMgr* inst = DevMgr_GetInstance();
    if (devId < inst->devCnt)
    {
        return &inst->devList[devId];
    }

    return NULL;
}

