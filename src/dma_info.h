/**
 * @file DMA的一些基本配置信息
 */

#ifndef FH228_DMA_INFO_H
#define FH228_DMA_INFO_H

/**
 * 发送用的DMA buf的基本信息
 */
typedef enum
{
    TX_NODE_CNT_HIGH            = 512,       ///< 高优先级dma节点数量
    TX_NODE_CNT_MID             = 512,       ///< 中优先级dma节点数量
    TX_NODE_CNT_LOW             = 512,       ///< 低优先级dma节点数量
    TX_NODE_CNT_PREPLAN         = 1024,      ///< 预配消息dma节点数量

    TX_NODE_SIZE_HIGH           = (16 * 1024), ///< 高优先级各个dma节点的大小
    TX_NODE_SIZE_MID            = (8 * 1024),  ///< 中优先级各个dma节点的大小
    TX_NODE_SIZE_LOW            = (64 * 1024), ///< 低优先级各个dma节点的大小
    TX_NODE_SIZE_PREPLAN        = (64 * 1024), ///< 预配消息dma节点大小

    TX_NODE_START_INDEX_HIGH    = 0, ///< 高优先级节点的开始下标
    TX_NODE_START_INDEX_MID     = (TX_NODE_START_INDEX_HIGH + TX_NODE_CNT_HIGH), ///< 中优先级节点的开始下标
    TX_NODE_START_INDEX_LOW     = (TX_NODE_START_INDEX_MID + TX_NODE_CNT_MID), ///< 低优先级节点的开始下标
    TX_NODE_START_INDEX_PREPLAN = (TX_NODE_START_INDEX_LOW + TX_NODE_CNT_LOW), ///< 周期消息的开始下标
} SendQueMgrConstValues;

typedef enum
{
    RX_NODE_CNT_HIGH     = 512,       ///< 高优先级dma节点数量
    RX_NODE_CNT_MID      = 512,       ///< 中优先级dma节点数量
    RX_NODE_CNT_LOW      = 512,       ///< 低优先级dma节点数量

    RX_NODE_SIZE_HIGH    = (16 * 1024), ///< 高优先级各个dma节点的大小
    RX_NODE_SIZE_MID     = (8 * 1024),  ///< 中优先级各个dma节点的大小
    RX_NODE_SIZE_LOW     = (64 * 1024), ///< 低优先级各个dma节点的大小
} RecvQueMgrConstValues;

// 各通道能发送的最大buf大小, 等于: 节点个数 x 每个节点大小 - W0 - W1
#define MAX_SEND_BUF_SIZE_PRIORITY_LOW  (TX_NODE_CNT_LOW * TX_NODE_SIZE_LOW - sizeof(W0) - sizeof(W1))
#define MAX_SEND_BUF_SIZE_PRIORITY_HIGH (TX_NODE_CNT_HIGH * TX_NODE_SIZE_HIGH - sizeof(W0) - sizeof(W1))

#endif
