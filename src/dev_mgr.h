#ifndef FH228_DEV_CONTEXT_MGR_H
#define FH228_DEV_CONTEXT_MGR_H

#include "common_header.h"
#include "dev_context.h"
#include "utils.h"

typedef struct
{
    bool        isInit;  ///< ��ʶ�Ƿ񱻳�ʼ����, ��ֹ������ʼ��
    int         devCnt;  ///< �豸�����ĸ���
    DevContext *devList; ///< �豸�������б�, һ���豸ӵ��һ��������, ����豸, ӵ�ж��������
} DevMgr;

extern DevMgr* g_devMgr;

bool DevMgr_Init();
void DevMgr_Uninit();
DevContext* DevMgr_GetDevice(const int devId);

#endif
