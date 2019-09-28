/**
 * @file
 * @brief �а�����, ������и�����ɸ�С��
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
    char*  data;    ///< dataָ��
    size_t size;    ///< ��¼data�Ĵ�С
    size_t offset;  ///< ƫ����, ��0��ʼ
} CsBufSpliterContext;

static void  Cs_BufSplitterAttach(CsBufSpliterContext* context, void* buf, const size_t size);
static void  Cs_BufSplitterSeek(CsBufSpliterContext* context, const size_t offsetBytes);
static void  Cs_BufSplitterSkip(CsBufSpliterContext* context, const size_t offsetBytes);
static void* Cs_BufSplitterGet(CsBufSpliterContext* context, const size_t bytes, size_t* realBytes);

//////////////////////////////////////////////////////////////////////////
// �����Ǻ���ʵ�ֲ���...

/**
 * @brief ������ָ����buf��ȥ
 * @param [in] context ������
 * @param [in] buf ָ����buf
 * @param [in] size buf�Ĵ�С
 */
static void Cs_BufSplitterAttach(CsBufSpliterContext* context, void* buf, const size_t size)
{
    context->data = (char*)buf;
    context->size = size;
    context->offset = 0;
}

/**
 * @brief ��ָ����ƫ��λ�ÿ�ʼ
 * @param [in] context ������
 * @param [in] offsetBytes ָ��Ҫƫ�Ƶ��ֽ���
 */
static void Cs_BufSplitterSeek(CsBufSpliterContext* context, const size_t offsetBytes)
{
    assert(offsetBytes <= context->size);
    context->offset = offsetBytes;
}

/**
 * @brief �ӵ�ǰλ��, �����ָ�����ֽ���
 * @param [in] context ������
 * @param [in] offsetBytes ָ����������ֽ���
 */
static void Cs_BufSplitterSkip(CsBufSpliterContext* context, const size_t offsetBytes)
{
    context->offset += offsetBytes;
    assert(context->offset <= context->size);
}

/**
 * @brief ��ȡָ����С��buf��
 * @param [in] context ������
 * @param [in] bytes ָ��Ҫ���ȡ�Ĵ�С
 * @param [out] realBytes ʵ�ʻ�ȡ���Ĵ�С
 * @return buf�ε��׵�ַ
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
