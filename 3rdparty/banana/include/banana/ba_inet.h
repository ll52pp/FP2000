#ifndef BA_INET_H_3ACF3322_59A6_4DCA_9B72_64E206B6B7E0
#define BA_INET_H_3ACF3322_59A6_4DCA_9B72_64E206B6B7E0

#if defined(_WIN32)
#if defined(WINNT)
#include "ba_inet_windrv.h"
#else
#include "ba_inet_win.h"
#endif
#elif defined(__VXWORKS__)
#include "ba_inet_vxworks.h"
#elif defined(LINUX_DRIVER) || defined(module_init)
#include "ba_inet_linux_drv.h"
#endif

#define Ba_HtonlStruct(st) Ba_Htonl(*(unsigned long*)&(st))
#define Ba_NtohlStruct(st) Ba_Ntohl(*(unsigned long*)&(st))
#define Ba_HtonsStruct(st) Ba_Htons(*(unsigned short*)&(st))
#define Ba_NtohsStruct(st) Ba_Ntohs(*(unsigned short*)&(st))

#endif
