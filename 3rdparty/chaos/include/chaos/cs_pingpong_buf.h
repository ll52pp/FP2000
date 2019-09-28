/**
 * @file
 * @brief ƹ��buffer, ��2��buffer������
 * @author yaoyuliang(765495939@qq.com)
 */


#ifndef CS_PINGPONG_H_6F279DA2_9DF8_49CE_A27C_E8D0D649DC82
#define CS_PINGPONG_H_6F279DA2_9DF8_49CE_A27C_E8D0D649DC82

#include "cs_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char* dbuf[2]; ///< ƹ��buffer
    size_t size; ///< ����buffer�Ĵ�С
    int curIndex; ///< ��ǰ�����±�, ���ڱ�ʶ��ƹ������
} CsPingPongBufContext;

/**
 * @brief ƹ��״̬, ���ڱ�ʶ�Ǵ���ƹ���Ǵ�����״̬
 */
typedef enum
{
    CS_PPB_PING = 0, ///< ����ƹ״̬
    CS_PPB_PONG = 1, ///< ������״̬
} CsPingPongState;

//////////////////////////////////////////////////////////////////////////
// ƹ��buf��ʵ�ֽӿ�
static void Cs_PingPongBufInit(CsPingPongBufContext* context, char* ping, char* pong, size_t size);
static void Cs_PingPongBufUninit(CsPingPongBufContext* context);
static void Cs_PingPongBufSwap(CsPingPongBufContext *context);
static CsPingPongState Cs_PingPongBufGetState(CsPingPongBufContext *context);

#define Cs_PingPongBufGetBuf(context) ((context)->dbuf[(context)->curIndex])
#define Cs_PingPongBufGetBufSize(context) ((context)->size)

//////////////////////////////////////////////////////////////////////////
// �����Ǻ���ʵ�ֲ���

/**
 * @brief ��ʼ��
 * @param [in] context ������
 * @param [in] ping ƹbuf�ĵ�ַ
 * @param [in] pong ��buf�ĵ�ַ
 * @param [in] size ��С, ����ƹ��buf��С, ��һ����
 */
static void Cs_PingPongBufInit(CsPingPongBufContext* context, char* ping, char* pong, size_t size)
{
    context->curIndex = 0;
    context->size = size;
    context->dbuf[0] = ping;
    context->dbuf[1] = pong;
}

/**
 * @brief ����ʼ��
 * @param [in] context ������
 */
static void Cs_PingPongBufUninit(CsPingPongBufContext* context)
{
    context->curIndex = 0;
    context->size = 0;
    context->dbuf[0] = NULL;
    context->dbuf[1] = NULL;
}

/**
 * @brief ��ƹ��2��buf֮�������л�
 * @param [in] context ������
 */
static void Cs_PingPongBufSwap(CsPingPongBufContext *context)
{
    context->curIndex = !context->curIndex;
}

/**
 * @brief ��ȡ��ǰ����ƹbuf, ��������buf
 * @param [in] context ������
 * @return ���ص�ǰ��ƹ��״̬
 */
static CsPingPongState Cs_PingPongBufGetState(CsPingPongBufContext *context)
{
    if (context->curIndex)
    {
        return CS_PPB_PONG;
    }

    return CS_PPB_PING;
}

#ifdef __cplusplus
}
#endif

#endif
