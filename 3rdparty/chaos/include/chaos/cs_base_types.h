#ifndef CS_BASE_TYPES_H_B6379C10_D27B_457E_84AF_FF4A3248BF20
#define CS_BASE_TYPES_H_B6379C10_D27B_457E_84AF_FF4A3248BF20

#if defined(_MSC_VER) && (_MSC_VER>=1700 ) && !defined(__cplusplus)
#include <stdbool.h>
#endif

#ifndef bool
#define bool int
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#if defined(_WIN32)
#include <stddef.h>
#elif defined(__VXWORKS__)
#include <vxworks.h>
#endif

#if defined(LINUX_DRIVER) || defined(module_init)
typedef char                CHAR;
typedef signed char         INT8;
typedef unsigned char       UCHAR;
typedef unsigned char       UINT8;
typedef unsigned char       BYTE;
typedef short               SHORT;
typedef signed short        INT16;
typedef unsigned short      USHORT;
typedef unsigned short      UINT16;
typedef unsigned short      WORD;
typedef int                 INT;
typedef signed int          INT32;
typedef unsigned int        UINT;
typedef unsigned int        UINT32;
typedef long                LONG;
typedef unsigned long       ULONG;
typedef unsigned long       DWORD;
typedef long long           LONGLONG;
typedef unsigned long long  ULONGLONG;
typedef unsigned long long  UINT64;
typedef long long           INT64;
#endif

// 一些常用的数据类型, 如UINT32, size_t, bool等的头文件
#endif
