#ifndef BA_MUTEX_H_52E42E48_2D63_4E33_9B5E_7436FBC7782B
#define BA_MUTEX_H_52E42E48_2D63_4E33_9B5E_7436FBC7782B

#if defined(_WIN32)
#if defined(WINNT)
#include "ba_mutex_windrv.h"
#else
#include "ba_mutex_win.h"
#endif
#elif defined(__VXWORKS__)
#include "ba_mutex_vxworks.h"
#elif defined(LINUX_DRIVER) || defined(module_init)
#include "ba_mutex_linux_drv.h"
#endif

#endif
