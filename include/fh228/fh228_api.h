#ifndef FH228_API_H_6687B536_6B40_4500_88B7_B5436D671E6C
#define FH228_API_H_6687B536_6B40_4500_88B7_B5436D671E6C

#include "fh228_def.h"

#ifdef __VXWORKS__
#define EXPORT_API extern
#else
#define EXPORT_API __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************************
 *    1.��ͷ�ļ�Ϊ��¶���û���API
 *    2.
 *    3.                                                                                     *
 *                                                                                         *
 *                                                                                         *
 * *****************************************************************************************/

/**
 * @brief ���豸
 * @param [in] devId �豸ID
 * @return �ɹ�����0
 */
EXPORT_API int Fh228_Open(const int devId);

/**
 * @brief �ر��豸
 * @param [in] devId �豸ID
 * @return �ɹ�����0
 */
EXPORT_API int Fh228_Close(const int devId);

/**
 * @brief �����豸
 * @param [in] devId �豸ID
 * @param [in] config ���õ�������Ϣ
 * @return �ɹ�����0
 */
EXPORT_API int Fh228_SetConfig(const int devId, const Fh228Config *config);

/**
 * @brief �������ݻ�����
 * @param [in] devId �豸ID
 * @param [in] subAddr �ӵ�ַ
 * @param [in] buf Ҫ���͵�����
 * @param [in] bytes ���ݵĴ�С
 * @return ���ͳ�ȥ���ֽ���, ��������ʱ, ��Ϊû������, ���Է��ص�ֵΪ0��ʾ�ɹ�
 */
EXPORT_API int Fh228_Send(const int devId, const unsigned int subAddr, const char *buf, const int bytes);

/**
 * @brief ��������
 * @param [in] devId �豸ID
 * @param [in] subAddr �ӵ�ַ
 * @param [in] buf ���ջ�����
 * @param [in] bytes ���ջ������Ĵ�С
 * @return �������յ������ݴ�С, ����ʱ����ֵС��0, ����0��ʾû�����ݵ�û�д���
 */
EXPORT_API int Fh228_Recv(const int devId, const unsigned int subAddr, char *buf, const int bytes);

/**
 * @brief ��������ָ��
 * @param [in] devId �豸ID
 * @param [in] cmds ����ָ������
 * @param [in] count ����ָ���������
 * @return �ɹ�����0
 */
EXPORT_API int Fh228_SetPeriodicMsgCmd(const int devId, Fh228PeriodicMsgCmd* cmds, const int count);

/**
 * @brief NT��Ԥ������
 * @param [in] devId �豸ID
 * @param [in] subAddr �ӵ�ַ
 * @param [in] buf ��������
 * @param [in] length ���ݴ�С
 * @param [in] bForce �Ƿ�ǿ�Ƹ���, 1ǿ�Ƹ���,�����ж���һ���Ƿ��Ѿ����͹�, 0��ǿ��,�����һ��û�з��͹�,���Ԥ��ʧ��
 * @return �ɹ�����0
 * @attention NC���벻Ҫ��������ӿ�
 */
EXPORT_API int Fh228_NtSetPreplanData(
    const int devId,
    const unsigned int subAddr,
    const char *buf,
    const int bytes,
    const int bForce
);

#ifdef __cplusplus
}
#endif

#endif
