#ifndef __FH228_READ_H__
#define __FH228_READ_H__

#include "common_header.h"
#include "dev_context.h"

/**
 * @brief 读取指定子地址所收到的数据
 * @param [in] dc 上下文
 * @param [in] subAddr 子地址
 * @param [out] buf 用于存放数据
 * @param [in] bytes buf缓存区的大小
 * @return 数据的实际大小
 */
int Read_FromChnQue(DevContext *dc, const uint32_t subAddr, char *buf, const int bytes);

/**
 * @brief 从接收队列里读数据
 * @param [in] cfg 上下文
 * @param [in] recvQueMgr 接收队列管理器
 * @param [in] priority 读指定优先级的队列
 * @return 成功返回true
 */
bool Read_FromRecvQue(CfgMgrCtx* cfg, RecvQueMgrContext *recvQueMgr, Fh228Priority priority);

#endif
