#ifndef BA_TASK_H_D10C2ECC_CC47_4A2B_8282_164AB3CA6566
#define BA_TASK_H_D10C2ECC_CC47_4A2B_8282_164AB3CA6566

#if defined(_WIN32)
#if defined(WINNT)
#include "ba_task_windrv.h"
#else
#include "ba_task_win.h"
#endif
#elif defined(__VXWORKS__)
#include "ba_task_vxworks.h"
#elif defined(LINUX_DRIVER) || defined(module_init)
#include "ba_task_linux_drv.h"
#endif

#endif
