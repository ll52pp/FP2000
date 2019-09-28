/**
 * @file
 * @brief 快速切包工具, 将大包切割成若干个小包
 * @author yaoyuliang(765495939@qq.com)
 */

#ifndef CS_FAST_BUF_SPLITTER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#define CS_FAST_BUF_SPLITTER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#include "cs_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char*  data;   ///< 数据首地址
    size_t size;   ///< 数据大小
    size_t offset; ///< 偏移量, 从0开始
} CsFastBufSpliterContext;

static void  Cs_FastBufSplitterAttach(CsFastBufSpliterContext* context, const void* buf, const size_t size);
static void* Cs_FastBufSplitterPop(CsFastBufSpliterContext* context, const size_t bytes);

/**
 * @fn Cs_FastBufSplitterGetOffset
 * @brief 获取当前的偏移量
 * @param [in] context 上下文
 * @return 偏移量
 */
#define Cs_FastBufSplitterGetOffset(context) ((context)->offset)

//////////////////////////////////////////////////////////////////////////
// 以下是函数实现部分...

/**
 * @brief 吸附到指定的buf上
 * @param [in] context 上下文
 * @param [in] buf 指定buf
 * @param [in] size buf大小
 */
static void Cs_FastBufSplitterAttach(CsFastBufSpliterContext* context, const void* buf, const size_t size)
{
    assert(NULL != context);
    context->data = (char*)buf;
    context->size = size;
    context->offset = 0;
}

/**
 * @brief 从尾部弹出指定大小的buf
 * @param [in] context 上下文
 * @param [in] bytes 要弹出的大小
 * @return 弹出的buf的地址
 */
static void* Cs_FastBufSplitterPop(CsFastBufSpliterContext* context, const size_t bytes)
{
    char* ret = context->data + context->offset;
    context->offset += bytes;
    assert(context->offset <= context->size);
    return ret;
}

#ifdef __cplusplus
}
#endif

#endif // CS_FAST_BUF_SPLITTER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
