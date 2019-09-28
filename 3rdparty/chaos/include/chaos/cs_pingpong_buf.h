/**
 * @file
 * @brief 乒乓buffer, 即2块buffer来回跳
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
    char* dbuf[2]; ///< 乒乓buffer
    size_t size; ///< 单个buffer的大小
    int curIndex; ///< 当前所处下标, 用于标识是乒还是乓
} CsPingPongBufContext;

/**
 * @brief 乒乓状态, 用于标识是处在乒还是处在乓状态
 */
typedef enum
{
    CS_PPB_PING = 0, ///< 处在乒状态
    CS_PPB_PONG = 1, ///< 处在乓状态
} CsPingPongState;

//////////////////////////////////////////////////////////////////////////
// 乒乓buf的实现接口
static void Cs_PingPongBufInit(CsPingPongBufContext* context, char* ping, char* pong, size_t size);
static void Cs_PingPongBufUninit(CsPingPongBufContext* context);
static void Cs_PingPongBufSwap(CsPingPongBufContext *context);
static CsPingPongState Cs_PingPongBufGetState(CsPingPongBufContext *context);

#define Cs_PingPongBufGetBuf(context) ((context)->dbuf[(context)->curIndex])
#define Cs_PingPongBufGetBufSize(context) ((context)->size)

//////////////////////////////////////////////////////////////////////////
// 以下是函数实现部分

/**
 * @brief 初始化
 * @param [in] context 上下文
 * @param [in] ping 乒buf的地址
 * @param [in] pong 乓buf的地址
 * @param [in] size 大小, 这里乒乓buf大小, 是一样的
 */
static void Cs_PingPongBufInit(CsPingPongBufContext* context, char* ping, char* pong, size_t size)
{
    context->curIndex = 0;
    context->size = size;
    context->dbuf[0] = ping;
    context->dbuf[1] = pong;
}

/**
 * @brief 反初始化
 * @param [in] context 上下文
 */
static void Cs_PingPongBufUninit(CsPingPongBufContext* context)
{
    context->curIndex = 0;
    context->size = 0;
    context->dbuf[0] = NULL;
    context->dbuf[1] = NULL;
}

/**
 * @brief 在乒乓2个buf之间来回切换
 * @param [in] context 上下文
 */
static void Cs_PingPongBufSwap(CsPingPongBufContext *context)
{
    context->curIndex = !context->curIndex;
}

/**
 * @brief 获取当前是在乒buf, 还是在乓buf
 * @param [in] context 上下文
 * @return 返回当前的乒乓状态
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
