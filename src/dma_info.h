/**
 * @file DMA��һЩ����������Ϣ
 */

#ifndef FH228_DMA_INFO_H
#define FH228_DMA_INFO_H

/**
 * �����õ�DMA buf�Ļ�����Ϣ
 */
typedef enum
{
    TX_NODE_CNT_HIGH            = 512,       ///< �����ȼ�dma�ڵ�����
    TX_NODE_CNT_MID             = 512,       ///< �����ȼ�dma�ڵ�����
    TX_NODE_CNT_LOW             = 512,       ///< �����ȼ�dma�ڵ�����
    TX_NODE_CNT_PREPLAN         = 1024,      ///< Ԥ����Ϣdma�ڵ�����

    TX_NODE_SIZE_HIGH           = (16 * 1024), ///< �����ȼ�����dma�ڵ�Ĵ�С
    TX_NODE_SIZE_MID            = (8 * 1024),  ///< �����ȼ�����dma�ڵ�Ĵ�С
    TX_NODE_SIZE_LOW            = (64 * 1024), ///< �����ȼ�����dma�ڵ�Ĵ�С
    TX_NODE_SIZE_PREPLAN        = (64 * 1024), ///< Ԥ����Ϣdma�ڵ��С

    TX_NODE_START_INDEX_HIGH    = 0, ///< �����ȼ��ڵ�Ŀ�ʼ�±�
    TX_NODE_START_INDEX_MID     = (TX_NODE_START_INDEX_HIGH + TX_NODE_CNT_HIGH), ///< �����ȼ��ڵ�Ŀ�ʼ�±�
    TX_NODE_START_INDEX_LOW     = (TX_NODE_START_INDEX_MID + TX_NODE_CNT_MID), ///< �����ȼ��ڵ�Ŀ�ʼ�±�
    TX_NODE_START_INDEX_PREPLAN = (TX_NODE_START_INDEX_LOW + TX_NODE_CNT_LOW), ///< ������Ϣ�Ŀ�ʼ�±�
} SendQueMgrConstValues;

typedef enum
{
    RX_NODE_CNT_HIGH     = 512,       ///< �����ȼ�dma�ڵ�����
    RX_NODE_CNT_MID      = 512,       ///< �����ȼ�dma�ڵ�����
    RX_NODE_CNT_LOW      = 512,       ///< �����ȼ�dma�ڵ�����

    RX_NODE_SIZE_HIGH    = (16 * 1024), ///< �����ȼ�����dma�ڵ�Ĵ�С
    RX_NODE_SIZE_MID     = (8 * 1024),  ///< �����ȼ�����dma�ڵ�Ĵ�С
    RX_NODE_SIZE_LOW     = (64 * 1024), ///< �����ȼ�����dma�ڵ�Ĵ�С
} RecvQueMgrConstValues;

// ��ͨ���ܷ��͵����buf��С, ����: �ڵ���� x ÿ���ڵ��С - W0 - W1
#define MAX_SEND_BUF_SIZE_PRIORITY_LOW  (TX_NODE_CNT_LOW * TX_NODE_SIZE_LOW - sizeof(W0) - sizeof(W1))
#define MAX_SEND_BUF_SIZE_PRIORITY_HIGH (TX_NODE_CNT_HIGH * TX_NODE_SIZE_HIGH - sizeof(W0) - sizeof(W1))

#endif
