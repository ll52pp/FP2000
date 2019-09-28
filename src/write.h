#ifndef SEND_IMPL_H_83A5F8EA_DEE9_4CA4_A3DB_9981D8BCBF19
#define SEND_IMPL_H_83A5F8EA_DEE9_4CA4_A3DB_9981D8BCBF19

#include "common_header.h"
#include "dev_context.h"

/**
 * @brief �����û�����, ��������
 * @return ���ͳ�ȥ�����ݴ�С
 * @attention ������ʱ����0��ʾ�ɹ�,��Ϊû������. ������ʱ,����ֵ�����ݴ�С���ʱ�ű�ʾ�ɹ�
 */
int Write_SendDataOrCmd(DevContext *dc, const uint32_t subAddr, const char *buf, const int bytes);

/**
 * @brief Ԥ������
 * @param [in] bForce �Ƿ�ǿ�Ƹ���, 1ǿ�Ƹ���,�����ж���һ���Ƿ��Ѿ����͹�, 0��ǿ��,�����һ��û�з��͹�,���Ԥ��ʧ��
 * @return �ɹ�����true, ʧ��false
 */
bool Write_SetPreplanData(DevContext *dc, const uint32_t subAddr, const char *buf, const uint32_t len, const bool bForce);

#endif
