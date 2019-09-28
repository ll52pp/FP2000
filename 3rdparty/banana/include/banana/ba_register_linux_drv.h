#ifndef BA_REGISTER_VXWORKS_H_F534FA25_16AB_44A8_8D33_70A9E586401E
#define BA_REGISTER_VXWORKS_H_F534FA25_16AB_44A8_8D33_70A9E586401E

#include <asm/io.h>

#ifdef __cplusplus
extern "C" {
#endif

static void Ba_RegisterWrite32(const void* address, const unsigned int value)
{
    writel(value, (void __iomem *)address);
}

static unsigned int Ba_RegisterRead32(const void* address)
{
    return readl(address);
}

#ifdef __cplusplus
}
#endif

#endif
