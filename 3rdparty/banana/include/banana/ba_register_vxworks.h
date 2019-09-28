#ifndef BA_REGISTER_VXWORKS_H_F534FA25_16AB_44A8_8D33_70A9E586401E
#define BA_REGISTER_VXWORKS_H_F534FA25_16AB_44A8_8D33_70A9E586401E

#ifdef __cplusplus
extern "C" {
#endif

#define Ba_ByteOrderSwap16(x) ((unsigned short int)((((unsigned short int)(x) & 0x00ff)<<8)| \
    (((unsigned short int)(x) & 0xff00) >> 8)))

#define Ba_ByteOrderSwap32(x) ((unsigned long int)((((unsigned long int)(x) & 0x000000ff)<<24)| \
    (((unsigned long int)(x) & 0x0000ff00) << 8)| \
    (((unsigned long int)(x) & 0x00ff0000) >> 8)| \
    (((unsigned long int)(x) & 0xff000000) >> 24)))

#if defined(__BIG_ENDIAN__)
#define Ba_HostValue2Register32(x) Ba_ByteOrderSwap32(x)
#define Ba_RegisterValue2Host32(x) Ba_ByteOrderSwap32(x)
#define BA_WRS_ASM(asm) _WRS_ASM(asm)
#else
#define Ba_HostValue2Register32(x) (x)
#define Ba_RegisterValue2Host32(x) (x)
#define BA_WRS_ASM(asm)
#endif


static void Ba_RegisterWrite32(const void* address, const unsigned int value)
{
    BA_WRS_ASM("eieio");
    *((unsigned int*)address) = Ba_HostValue2Register32(value);
}

static unsigned int Ba_RegisterRead32(const void* address)
{
    BA_WRS_ASM("eieio");
    return Ba_RegisterValue2Host32(*(unsigned int*)address);
}

#ifdef __cplusplus
}
#endif

#endif
