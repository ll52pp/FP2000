/**
 * @file
 * @brief ��bufд����
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
    size_t offset; ///< ƫ����, ��0��ʼ
} CsBufWriterContext;

static void  Cs_BufWriterAttach(CsBufWriterContext* context, void* buf, const size_t size);
static bool  Cs_BufWriterPushStr(CsBufWriterContext* context, const void* buf, const size_t size);
static void* Cs_BufWriterGetBuf(CsBufWriterContext* context, size_t* size);

/**
 * @fn Cs_BufWriterGetOffset
 * @brief ��ȡ��ǰλ�õ�ƫ����
 * @param [in] context ������
 * @return ����ƫ����
 */
#define Cs_BufWriterGetOffset(context) ((size_t)((context)->offset))

/**
 * @fn Cs_BufWriterPush
 * @brief �򻺴���д��һ��ָ�����͵�����
 * @param [in] context ������
 * @param [in] ValueType ���ݵ�����
 * @param [in] value ����
 */
#define Cs_BufWriterPush(context, ValueType, value) \
    { \
        ValueType* tmp = (ValueType*)((context)->data + (context)->offset); \
        *tmp = value; \
        (context)->offset += sizeof(ValueType); \
    }

//////////////////////////////////////////////////////////////////////////
// �����Ǻ���ʵ�ֲ���...

/**
 * @brief ������ָ����buf��
 * @param [in] context ������
 * @param [in] buf ָ����buf
 * @param [in] size buf�Ĵ�С
 */
static void Cs_BufWriterAttach(CsBufWriterContext* context, void* buf, const size_t size)
{
    context->data = (char*)buf;
    context->size = size;
    context->offset = 0;
}

/**
 * @brief ���׷��д��ָ�����ȵ��ַ���
 * @param [in] context ������
 * @param [in] buf ָ��Ҫ׷�ӵ��ַ���
 * @param [in] size �ַ����Ĵ�С
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
 * @brief ��ȡ����
 * @param [in] context ������
 * @param [out] size �������ݵĴ�С
 * @return ���ݵ��׵�ַ
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
