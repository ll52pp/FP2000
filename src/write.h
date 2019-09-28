#ifndef SEND_IMPL_H_83A5F8EA_DEE9_4CA4_A3DB_9981D8BCBF19
#define SEND_IMPL_H_83A5F8EA_DEE9_4CA4_A3DB_9981D8BCBF19

#include "common_header.h"
#include "dev_context.h"

/**
 * @brief 发送用户数据, 或者命令
 * @return 发送出去的数据大小
 * @attention 发命令时返回0表示成功,因为没有数据. 发数据时,返回值和数据大小相等时才表示成功
 */
int Write_SendDataOrCmd(DevContext *dc, const uint32_t subAddr, const char *buf, const int bytes);

/**
 * @brief 预配数据
 * @param [in] bForce 是否强制更新, 1强制更新,将不判断上一条是否已经发送过, 0非强制,如果上一条没有发送过,则会预配失败
 * @return 成功返回true, 失败false
 */
bool Write_SetPreplanData(DevContext *dc, const uint32_t subAddr, const char *buf, const uint32_t len, const bool bForce);

#endif
