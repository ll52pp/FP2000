#ifndef UTILS_H_5747FE94_4660_4B11_8C89_9400B6A0D323
#define UTILS_H_5747FE94_4660_4B11_8C89_9400B6A0D323

#include "common_header.h"
#include "struct.h"
#include "fpga_reg_addr.h"

#define CastStruct2UInt32(st) (*((unsigned int*)(&(st))))
#define ZeroStruct(st) memset(&(st), 0, sizeof(st))

typedef struct
{
    int busNo;
    int deviceNo;
    int funcNo;
    UINT32 membaseCsr;
    UINT8 irq;
} DevBaseInfo;

int      Utils_UniqueInt(int* arr, int n);
void     Utils_BubleSortInt(int* arr, int n);
void     Utils_DoEmptyLoop(int count);
int      Utils_FindDev(const int vendorId, const int deviceId, DevBaseInfo* info, const int infoCnt);
uint8_t  Utils_FpgaHashCode(unsigned int high, unsigned int low);

#endif
