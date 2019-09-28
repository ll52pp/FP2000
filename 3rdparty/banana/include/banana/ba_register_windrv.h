#pragma once
#include <wdm.h>

#ifdef __cplusplus
extern "C" {
#endif

static void Ba_RegisterWrite32(const void* address, const unsigned int value)
{
    WRITE_REGISTER_ULONG((PULONG)address, (ULONG)value);
}

static unsigned int Ba_RegisterRead32(const void* address)
{
    return (unsigned int)READ_REGISTER_ULONG((PULONG)address);
}

#ifdef __cplusplus
}
#endif
