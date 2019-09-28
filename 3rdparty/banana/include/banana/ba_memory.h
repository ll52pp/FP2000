#ifndef BA_MEMORY_H_52E42E48_2D63_4E33_9B5E_7436FBC7782B
#define BA_MEMORY_H_52E42E48_2D63_4E33_9B5E_7436FBC7782B

#if defined(_WIN32)
#if defined(WIN_DRIVER) || defined(_NTDDK_) || defined(_WDF_H_) || defined(WDFAPI)
#include <wdm.h>
#define Ba_Malloc(size) ExAllocatePoolWithTag(NonPagedPool, (size), 'cshm')
#define Ba_Free(ptr) ExFreePoolWithTag(ptr, 'cshm')
#else
#include <stdlib.h>
#define Ba_Malloc malloc
#define Ba_Free free
#endif
#elif defined(__VXWORKS__)
#include <stdlib.h>
#define Ba_Malloc malloc
#define Ba_Free free
#elif defined(LINUX_DRIVER) || defined(module_init)
#define Ba_Malloc(size) kmalloc((size), GFP_KERNEL)
#define Ba_Free kfree
#else
#error "banana does not support on your platform"
#endif

#endif
