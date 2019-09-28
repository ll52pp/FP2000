#ifndef __FH228_READ_H__
#define __FH228_READ_H__

#include "common_header.h"
#include "dev_context.h"

/**
 * @brief ��ȡָ���ӵ�ַ���յ�������
 * @param [in] dc ������
 * @param [in] subAddr �ӵ�ַ
 * @param [out] buf ���ڴ������
 * @param [in] bytes buf�������Ĵ�С
 * @return ���ݵ�ʵ�ʴ�С
 */
int Read_FromChnQue(DevContext *dc, const uint32_t subAddr, char *buf, const int bytes);

/**
 * @brief �ӽ��ն����������
 * @param [in] cfg ������
 * @param [in] recvQueMgr ���ն��й�����
 * @param [in] priority ��ָ�����ȼ��Ķ���
 * @return �ɹ�����true
 */
bool Read_FromRecvQue(CfgMgrCtx* cfg, RecvQueMgrContext *recvQueMgr, Fh228Priority priority);

#endif
