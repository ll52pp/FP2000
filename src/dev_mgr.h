#ifndef FH228_DEV_CONTEXT_MGR_H
#define FH228_DEV_CONTEXT_MGR_H

#include "common_header.h"
#include "dev_context.h"
#include "utils.h"

typedef struct
{
    bool        isInit;  ///< 标识是否被初始化过, 防止反复初始化
    int         devCnt;  ///< 设备上下文个数
    DevContext *devList; ///< 设备上下文列表, 一个设备拥有一个上下文, 多个设备, 拥有多个上下文
} DevMgr;

extern DevMgr* g_devMgr;

bool DevMgr_Init();
void DevMgr_Uninit();
DevContext* DevMgr_GetDevice(const int devId);

#endif
