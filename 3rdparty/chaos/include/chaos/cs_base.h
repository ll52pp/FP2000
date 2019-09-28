/**
 * @file
 * @brief base dependences of chaos
 *
 * 为了保证高性能, 高效率的执行, 并没有做过多容错处理
 * 我们假设你对底层数据结果非常了解, 使用时请小心.
 */

#ifndef CS_BASE_H_718F1719_5495_4643_AC44_A3DA45190E47
#define CS_BASE_H_718F1719_5495_4643_AC44_A3DA45190E47

#define CHAOS_VERSION 0x020000
#define CHAOS_VERSION_STRING "2.0.0"


#include "cs_base_types.h"

#if defined(_WIN32)
#include <assert.h>
#include <string.h>
#if defined(WIN_DRIVER) || defined(_NTDDK_) || defined(_WDF_H_) || defined(WDFAPI)
#include <wdm.h>
#define Cs_Malloc(size) ExAllocatePoolWithTag(NonPagedPool, (size), 'cshm')
#define Cs_Free(ptr) ExFreePoolWithTag(ptr, 'cshm')
#else
#include <stdlib.h>
#define Cs_Malloc malloc
#define Cs_Free free
#endif
#elif defined(__VXWORKS__)
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#define Cs_Malloc malloc
#define Cs_Free free
#elif defined(LINUX_DRIVER) || defined(module_init)
#include<linux/slab.h>
#define assert(cond)                                              \
{                                                                 \
    if (!(cond))                                                  \
       printk(KERN_ALERT "ASSERT: %s(%u)\n", __FILE__, __LINE__); \
}
#define Cs_Malloc(size) kmalloc((size), GFP_KERNEL)
#define Cs_Free kfree
#else
#error "chaos lib does not support on your platform"
#endif

#endif /// CS_BASE_H_718F1719_5495_4643_AC44_A3DA45190E47
