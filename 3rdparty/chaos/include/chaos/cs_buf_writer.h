/**
 * @file
 * @brief 往buf写数据
 * @author yaoyuliang(765495939@qq.com)
 **/

#ifndef CS_BUF_WRITER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#define CS_BUF_WRITER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
#include "cs_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char*  data;
    size_t size;
    size_t offset; ///< 偏移量, 从0开始
} CsBufWriterContext;

static void  Cs_BufWriterAttach(CsBufWriterContext* context, void* buf, const size_t size);
static bool  Cs_BufWriterPushStr(CsBufWriterContext* context, const void* buf, const size_t size);
static void* Cs_BufWriterGetBuf(CsBufWriterContext* context, size_t* size);

/**
 * @fn Cs_BufWriterGetOffset
 * @brief 获取当前位置的偏移量
 * @param [in] context 上下文
 * @return 返回偏移量
 */
#define Cs_BufWriterGetOffset(context) ((size_t)((context)->offset))

/**
 * @fn Cs_BufWriterPush
 * @brief 向缓存区写入一个指定类型的数据
 * @param [in] context 上下文
 * @param [in] ValueType 数据的类型
 * @param [in] value 数据
 */
#define Cs_BufWriterPush(context, ValueType, value) \
    { \
        ValueType* tmp = (ValueType*)((context)->data + (context)->offset); \
        *tmp = value; \
        (context)->offset += sizeof(ValueType); \
    }

//////////////////////////////////////////////////////////////////////////
// 以下是函数实现部分...

/**
 * @brief 吸附到指定的buf上
 * @param [in] context 上下文
 * @param [in] buf 指定的buf
 * @param [in] size buf的大小
 */
static void Cs_BufWriterAttach(CsBufWriterContext* context, void* buf, const size_t size)
{
    context->data = (char*)buf;
    context->size = size;
    context->offset = 0;
}

/**
 * @brief 向后追加写入指定长度的字符串
 * @param [in] context 上下文
 * @param [in] buf 指定要追加的字符串
 * @param [in] size 字符串的大小
 */
static bool Cs_BufWriterPushStr(CsBufWriterContext* context, const void* buf, const size_t size)
{
    if (buf && ((context->offset + size) <= context->size))
    {
        memcpy(context->data + context->offset, buf, size);
        context->offset += size;
        return true;
    }
    return false;
}

/**
 * @brief 读取数据
 * @param [in] context 上下文
 * @param [out] size 返回数据的大小
 * @return 数据的首地址
 */
static void* Cs_BufWriterGetBuf(CsBufWriterContext* context, size_t* size)
{
    if (size)
    {
        *size = context->size;
    }
    return context->data;
}

#ifdef __cplusplus
}
#endif

#endif // CS_BUF_WRITER_H_3935E820_DD02_4DD7_AC55_9F92796BC977
