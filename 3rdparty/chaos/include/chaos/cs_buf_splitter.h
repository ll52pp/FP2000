/**
 * @file
 * @brief 切包工具, 将大包切割成若干个小包
 * @author yaoyuliang(765495939@qq.com)
 **/

#ifndef CS_BUF_SPLITTER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#define CS_BUF_SPLITTER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#include "cs_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char*  data;    ///< data指针
    size_t size;    ///< 记录data的大小
    size_t offset;  ///< 偏移量, 从0开始
} CsBufSpliterContext;

static void  Cs_BufSplitterAttach(CsBufSpliterContext* context, void* buf, const size_t size);
static void  Cs_BufSplitterSeek(CsBufSpliterContext* context, const size_t offsetBytes);
static void  Cs_BufSplitterSkip(CsBufSpliterContext* context, const size_t offsetBytes);
static void* Cs_BufSplitterGet(CsBufSpliterContext* context, const size_t bytes, size_t* realBytes);

//////////////////////////////////////////////////////////////////////////
// 以下是函数实现部分...

/**
 * @brief 吸附到指定的buf上去
 * @param [in] context 上下文
 * @param [in] buf 指定的buf
 * @param [in] size buf的大小
 */
static void Cs_BufSplitterAttach(CsBufSpliterContext* context, void* buf, const size_t size)
{
    context->data = (char*)buf;
    context->size = size;
    context->offset = 0;
}

/**
 * @brief 从指定的偏移位置开始
 * @param [in] context 上下文
 * @param [in] offsetBytes 指定要偏移的字节数
 */
static void Cs_BufSplitterSeek(CsBufSpliterContext* context, const size_t offsetBytes)
{
    assert(offsetBytes <= context->size);
    context->offset = offsetBytes;
}

/**
 * @brief 从当前位置, 向后跳指定的字节数
 * @param [in] context 上下文
 * @param [in] offsetBytes 指定向后跳的字节数
 */
static void Cs_BufSplitterSkip(CsBufSpliterContext* context, const size_t offsetBytes)
{
    context->offset += offsetBytes;
    assert(context->offset <= context->size);
}

/**
 * @brief 获取指定大小的buf段
 * @param [in] context 上下文
 * @param [in] bytes 指定要想获取的大小
 * @param [out] realBytes 实际获取到的大小
 * @return buf段的首地址
 */
static void* Cs_BufSplitterGet(CsBufSpliterContext* context, const size_t bytes, size_t* realBytes)
{
    if (context->offset < context->size)
    {
        size_t oldOffset = context->offset;
        size_t newOffset = oldOffset + bytes;
        size_t len = bytes;

        if (newOffset <= context->size)
        {
            context->offset = newOffset;
        }
        else
        {
            len = context->size - oldOffset;
            context->offset = context->size;
        }

        if (realBytes)
        {
            *realBytes = len;
        }

        return (context->data + oldOffset);
    }

    return NULL;
}

#ifdef __cplusplus
}
#endif

#endif // CS_BUF_SPLITTER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
