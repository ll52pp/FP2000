#ifndef BA_INET_LINUX_DRV_010F09A5_3012_40CA_BA92_23CC24F86474
#define BA_INET_LINUX_DRV_010F09A5_3012_40CA_BA92_23CC24F86474

#include "ba_base_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BigLittleSwap16(A)  ((((UINT16)(A) & 0xff00) >> 8) | \
                            (((UINT16)(A) & 0x00ff) << 8))

#define BigLittleSwap32(A)  ((((UINT32)(A) & 0xff000000) >> 24) | \
                            (((UINT32)(A) & 0x00ff0000) >> 8) | \
                            (((UINT32)(A) & 0x0000ff00) << 8) | \
                            (((UINT32)(A) & 0x000000ff) << 24))

static int _Ba_CheckCpuEndian(void)
{
    union
    {
        unsigned long int i;
        unsigned char s[4];
    }c;
    c.i = 0x12345678;
    return (0x12 == c.s[0]);
}

static unsigned long Ba_Htonl(unsigned long h)
{
    return _Ba_CheckCpuEndian() ? h : BigLittleSwap32(h);
}

static unsigned long Ba_Ntohl(unsigned long n)
{
    return _Ba_CheckCpuEndian() ? n : BigLittleSwap32(n);
}

static unsigned short Ba_Htons(unsigned short h)
{
    return _Ba_CheckCpuEndian() ? h : BigLittleSwap16(h);
}

static unsigned short Ba_Ntohs(unsigned short n)
{
    return _Ba_CheckCpuEndian() ? n : BigLittleSwap16(n);
}

#ifdef __cplusplus
}
#endif

#endif
