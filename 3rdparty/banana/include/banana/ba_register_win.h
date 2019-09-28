#pragma once
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

static void Ba_RegisterWrite32(const void* address, const unsigned int value)
{
    printf("[banan] 设置寄存器, 参数.址地=%p, 参数.值=0x%08x\n", address, value);
}

static unsigned int Ba_RegisterRead32(const void* address)
{
    printf("[banan] 设置寄存器, 参数.址地=%p, 返回值=0x%08x\n", address, 0);
    return 0;
}

#ifdef __cplusplus
}
#endif
