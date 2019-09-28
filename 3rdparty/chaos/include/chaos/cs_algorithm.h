#ifndef CS_ALGORITHM_H_718F1719_5495_4643_AC44_A3DA45190E47
#define CS_ALGORITHM_H_718F1719_5495_4643_AC44_A3DA45190E47
#include "cs_base.h"
#include "cs_crc32.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief 计算字节对齐
 * @param [in] size 数据大小
 * @param [in] align 对齐大小
 * @return 对齐后的大小
 */
static size_t Cs_CalcByteAlignment(const size_t size, const size_t align) {
    assert(align <= (30 * 1024 * 1024));
    return ((size + align - 1) & (~(align - 1)));
}


#ifdef __cplusplus
}
#endif

#endif
