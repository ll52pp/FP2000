#pragma once
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

static void Ba_RegisterWrite32(const void* address, const unsigned int value)
{
    printf("[banan] ���üĴ���, ����.ַ��=%p, ����.ֵ=0x%08x\n", address, value);
}

static unsigned int Ba_RegisterRead32(const void* address)
{
    printf("[banan] ���üĴ���, ����.ַ��=%p, ����ֵ=0x%08x\n", address, 0);
    return 0;
}

#ifdef __cplusplus
}
#endif
