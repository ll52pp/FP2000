#ifndef CS_ALGORITHM_H_718F1719_5495_4643_AC44_A3DA45190E47
#define CS_ALGORITHM_H_718F1719_5495_4643_AC44_A3DA45190E47
#include "cs_base.h"
#include "cs_crc32.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief �����ֽڶ���
 * @param [in] size ���ݴ�С
 * @param [in] align �����С
 * @return �����Ĵ�С
 */
static size_t Cs_CalcByteAlignment(const size_t size, const size_t align) {
    assert(align <= (30 * 1024 * 1024));
    return ((size + align - 1) & (~(align - 1)));
}


#ifdef __cplusplus
}
#endif

#endif
