#ifndef BA_REGISTER_H_0C04993E_820F_46E0_9588_CD262C1D9271
#define BA_REGISTER_H_0C04993E_820F_46E0_9588_CD262C1D9271

#if defined(_WIN32)
#if defined(WINNT)
#include "ba_register_windrv.h"
#else
#include "ba_register_win.h"
#endif
#elif defined(__VXWORKS__)
#include "ba_register_vxworks.h"
#elif defined(LINUX_DRIVER) || defined(module_init)
#include "ba_register_linux_drv.h"
#endif

#define Ba_RegisterWrite32Struct(address, st) Ba_RegisterWrite32(address, *((unsigned int*)(&(st))))
#define Ba_RegisterRead32Struct(address, pSt) ((*(unsigned int*)pSt) = Ba_RegisterRead32(address))

#endif
