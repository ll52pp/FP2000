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
 *    1.此头文件为暴露给用户的API
 *    2.
 *    3.                                                                                     *
 *                                                                                         *
 *                                                                                         *
 * *****************************************************************************************/

/**
 * @brief 打开设备
 * @param [in] devId 设备ID
 * @return 成功返回0
 */
EXPORT_API int Fh228_Open(const int devId);

/**
 * @brief 关闭设备
 * @param [in] devId 设备ID
 * @return 成功返回0
 */
EXPORT_API int Fh228_Close(const int devId);

/**
 * @brief 配置设备
 * @param [in] devId 设备ID
 * @param [in] config 设置的配置信息
 * @return 成功返回0
 */
EXPORT_API int Fh228_SetConfig(const int devId, const Fh228Config *config);

/**
 * @brief 发送数据或命令
 * @param [in] devId 设备ID
 * @param [in] subAddr 子地址
 * @param [in] buf 要发送的数据
 * @param [in] bytes 数据的大小
 * @return 发送出去的字节数, 发送命令时, 因为没有数据, 所以返回的值为0表示成功
 */
EXPORT_API int Fh228_Send(const int devId, const unsigned int subAddr, const char *buf, const int bytes);

/**
 * @brief 接收数据
 * @param [in] devId 设备ID
 * @param [in] subAddr 子地址
 * @param [in] buf 接收缓存区
 * @param [in] bytes 接收缓存区的大小
 * @return 真正接收到的数据大小, 出错时返回值小于0, 等于0表示没有数据但没有错误
 */
EXPORT_API int Fh228_Recv(const int devId, const unsigned int subAddr, char *buf, const int bytes);

/**
 * @brief 配置周期指令
 * @param [in] devId 设备ID
 * @param [in] cmds 周期指令数组
 * @param [in] count 周期指令数组个数
 * @return 成功返回0
 */
EXPORT_API int Fh228_SetPeriodicMsgCmd(const int devId, Fh228PeriodicMsgCmd* cmds, const int count);

/**
 * @brief NT端预配数据
 * @param [in] devId 设备ID
 * @param [in] subAddr 子地址
 * @param [in] buf 数据内容
 * @param [in] length 数据大小
 * @param [in] bForce 是否强制更新, 1强制更新,将不判断上一条是否已经发送过, 0非强制,如果上一条没有发送过,则会预配失败
 * @return 成功返回0
 * @attention NC端请不要调用这个接口
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
