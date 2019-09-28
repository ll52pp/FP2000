/**
 * @file
 * @brief �����а�����, ������и�����ɸ�С��
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
    char*  data;   ///< �����׵�ַ
    size_t size;   ///< ���ݴ�С
    size_t offset; ///< ƫ����, ��0��ʼ
} CsFastBufSpliterContext;

static void  Cs_FastBufSplitterAttach(CsFastBufSpliterContext* context, const void* buf, const size_t size);
static void* Cs_FastBufSplitterPop(CsFastBufSpliterContext* context, const size_t bytes);

/**
 * @fn Cs_FastBufSplitterGetOffset
 * @brief ��ȡ��ǰ��ƫ����
 * @param [in] context ������
 * @return ƫ����
 */
#define Cs_FastBufSplitterGetOffset(context) ((context)->offset)

//////////////////////////////////////////////////////////////////////////
// �����Ǻ���ʵ�ֲ���...

/**
 * @brief ������ָ����buf��
 * @param [in] context ������
 * @param [in] buf ָ��buf
 * @param [in] size buf��С
 */
static void Cs_FastBufSplitterAttach(CsFastBufSpliterContext* context, const void* buf, const size_t size)
{
    assert(NULL != context);
    context->data = (char*)buf;
    context->size = size;
    context->offset = 0;
}

/**
 * @brief ��β������ָ����С��buf
 * @param [in] context ������
 * @param [in] bytes Ҫ�����Ĵ�С
 * @return ������buf�ĵ�ַ
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
