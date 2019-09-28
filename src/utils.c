#include "utils.h"

typedef union
{
    struct
    {
#ifndef __BIG_ENDIAN__
        UCHAR b0 : 1;
        UCHAR b1 : 1;
        UCHAR b2 : 1;
        UCHAR b3 : 1;
        UCHAR b4 : 1;
        UCHAR b5 : 1;
        UCHAR b6 : 1;
        UCHAR b7 : 1;
#else
        UCHAR b7 : 1;
        UCHAR b6 : 1;
        UCHAR b5 : 1;
        UCHAR b4 : 1;
        UCHAR b3 : 1;
        UCHAR b2 : 1;
        UCHAR b1 : 1;
        UCHAR b0 : 1;
#endif
    } u;
    UCHAR value;
} UtilsFpgaHash8Bit;

typedef union
{
    struct
    {
#ifndef __BIG_ENDIAN__
        UINT64 b0 : 1;
        UINT64 b1 : 1;
        UINT64 b2 : 1;
        UINT64 b3 : 1;
        UINT64 b4 : 1;
        UINT64 b5 : 1;
        UINT64 b6 : 1;
        UINT64 b7 : 1;
        UINT64 b8 : 1;
        UINT64 b9 : 1;
        UINT64 b10 : 1;
        UINT64 b11 : 1;
        UINT64 b12 : 1;
        UINT64 b13 : 1;
        UINT64 b14 : 1;
        UINT64 b15 : 1;
        UINT64 b16 : 1;
        UINT64 b17 : 1;
        UINT64 b18 : 1;
        UINT64 b19 : 1;
        UINT64 b20 : 1;
        UINT64 b21 : 1;
        UINT64 b22 : 1;
        UINT64 b23 : 1;
        UINT64 b24 : 1;
        UINT64 b25 : 1;
        UINT64 b26 : 1;
        UINT64 b27 : 1;
        UINT64 b28 : 1;
        UINT64 b29 : 1;
        UINT64 b30 : 1;
        UINT64 b31 : 1;
        UINT64 b32 : 1;
        UINT64 b33 : 1;
        UINT64 b34 : 1;
        UINT64 b35 : 1;
        UINT64 b36 : 1;
        UINT64 b37 : 1;
        UINT64 b38 : 1;
        UINT64 b39 : 1;
        UINT64 b40 : 1;
        UINT64 b41 : 1;
        UINT64 b42 : 1;
        UINT64 b43 : 1;
        UINT64 b44 : 1;
        UINT64 b45 : 1;
        UINT64 b46 : 1;
        UINT64 b47 : 1;
        UINT64 b48 : 1;
        UINT64 b49 : 1;
        UINT64 b50 : 1;
        UINT64 b51 : 1;
        UINT64 b52 : 1;
        UINT64 b53 : 1;
        UINT64 b54 : 1;
        UINT64 b55 : 1;
        UINT64 b56 : 1;
        UINT64 b57 : 1;
        UINT64 b58 : 1;
        UINT64 b59 : 1;
        UINT64 b60 : 1;
        UINT64 b61 : 1;
        UINT64 b62 : 1;
        UINT64 b63 : 1;
#else
        UINT64 b31 : 1;
        UINT64 b30 : 1;
        UINT64 b29 : 1;
        UINT64 b28 : 1;
        UINT64 b27 : 1;
        UINT64 b26 : 1;
        UINT64 b25 : 1;
        UINT64 b24 : 1;
        UINT64 b23 : 1;
        UINT64 b22 : 1;
        UINT64 b21 : 1;
        UINT64 b20 : 1;
        UINT64 b19 : 1;
        UINT64 b18 : 1;
        UINT64 b17 : 1;
        UINT64 b16 : 1;
        UINT64 b15 : 1;
        UINT64 b14 : 1;
        UINT64 b13 : 1;
        UINT64 b12 : 1;
        UINT64 b11 : 1;
        UINT64 b10 : 1;
        UINT64 b9 : 1;
        UINT64 b8 : 1;
        UINT64 b7 : 1;
        UINT64 b6 : 1;
        UINT64 b5 : 1;
        UINT64 b4 : 1;
        UINT64 b3 : 1;
        UINT64 b2 : 1;
        UINT64 b1 : 1;
        UINT64 b0 : 1;
        UINT64 b63 : 1;
        UINT64 b62 : 1;
        UINT64 b61 : 1;
        UINT64 b60 : 1;
        UINT64 b59 : 1;
        UINT64 b58 : 1;
        UINT64 b57 : 1;
        UINT64 b56 : 1;
        UINT64 b55 : 1;
        UINT64 b54 : 1;
        UINT64 b53 : 1;
        UINT64 b52 : 1;
        UINT64 b51 : 1;
        UINT64 b50 : 1;
        UINT64 b49 : 1;
        UINT64 b48 : 1;
        UINT64 b47 : 1;
        UINT64 b46 : 1;
        UINT64 b45 : 1;
        UINT64 b44 : 1;
        UINT64 b43 : 1;
        UINT64 b42 : 1;
        UINT64 b41 : 1;
        UINT64 b40 : 1;
        UINT64 b39 : 1;
        UINT64 b38 : 1;
        UINT64 b37 : 1;
        UINT64 b36 : 1;
        UINT64 b35 : 1;
        UINT64 b34 : 1;
        UINT64 b33 : 1;
        UINT64 b32 : 1;
#endif
    } u;
    struct
    {
        ULONG low;
        ULONG high;
    } part;
    UINT64 value;
} UtilsFpgaHash64Bit;

uint8_t Utils_FpgaHashCode(unsigned int high, unsigned int low)
{
    UtilsFpgaHash8Bit r;
    UtilsFpgaHash64Bit d, c;

    d.part.high = high;
    d.part.low = low;
    c.value = 0;
    r.value = 0;
    r.u.b0 = (UINT8)(d.u.b61 ^ d.u.b60 ^ d.u.b56 ^ d.u.b54 ^ d.u.b53 ^ d.u.b52 ^ d.u.b51 ^ d.u.b49 ^ d.u.b48 ^ d.u.b47 ^ d.u.b44 ^ d.u.b40 ^ d.u.b35 ^ d.u.b34 ^ d.u.b32 ^ d.u.b30 ^ d.u.b29 ^ d.u.b27 ^ d.u.b23 ^ d.u.b22 ^ d.u.b21 ^ d.u.b20 ^ d.u.b14 ^ d.u.b11 ^ d.u.b10 ^ d.u.b8 ^ d.u.b7 ^ d.u.b2 ^ d.u.b0 ^ c.u.b3 ^ c.u.b4);
    r.u.b1 = (UINT8)(d.u.b62 ^ d.u.b60 ^ d.u.b57 ^ d.u.b56 ^ d.u.b55 ^ d.u.b51 ^ d.u.b50 ^ d.u.b47 ^ d.u.b45 ^ d.u.b44 ^ d.u.b41 ^ d.u.b40 ^ d.u.b36 ^ d.u.b34 ^ d.u.b33 ^ d.u.b32 ^ d.u.b31 ^ d.u.b29 ^ d.u.b28 ^ d.u.b27 ^ d.u.b24 ^ d.u.b20 ^ d.u.b15 ^ d.u.b14 ^ d.u.b12 ^ d.u.b10 ^ d.u.b9 ^ d.u.b7 ^ d.u.b3 ^ d.u.b2 ^ d.u.b1 ^ d.u.b0 ^ c.u.b0 ^ c.u.b3 ^ c.u.b5);
    r.u.b2 = (UINT8)(d.u.b63 ^ d.u.b61 ^ d.u.b58 ^ d.u.b57 ^ d.u.b56 ^ d.u.b52 ^ d.u.b51 ^ d.u.b48 ^ d.u.b46 ^ d.u.b45 ^ d.u.b42 ^ d.u.b41 ^ d.u.b37 ^ d.u.b35 ^ d.u.b34 ^ d.u.b33 ^ d.u.b32 ^ d.u.b30 ^ d.u.b29 ^ d.u.b28 ^ d.u.b25 ^ d.u.b21 ^ d.u.b16 ^ d.u.b15 ^ d.u.b13 ^ d.u.b11 ^ d.u.b10 ^ d.u.b8 ^ d.u.b4 ^ d.u.b3 ^ d.u.b2 ^ d.u.b1 ^ c.u.b0 ^ c.u.b1 ^ c.u.b4 ^ c.u.b6);
    r.u.b3 = (UINT8)(d.u.b62 ^ d.u.b61 ^ d.u.b60 ^ d.u.b59 ^ d.u.b58 ^ d.u.b57 ^ d.u.b56 ^ d.u.b54 ^ d.u.b51 ^ d.u.b48 ^ d.u.b46 ^ d.u.b44 ^ d.u.b43 ^ d.u.b42 ^ d.u.b40 ^ d.u.b38 ^ d.u.b36 ^ d.u.b33 ^ d.u.b32 ^ d.u.b31 ^ d.u.b27 ^ d.u.b26 ^ d.u.b23 ^ d.u.b21 ^ d.u.b20 ^ d.u.b17 ^ d.u.b16 ^ d.u.b12 ^ d.u.b10 ^ d.u.b9 ^ d.u.b8 ^ d.u.b7 ^ d.u.b5 ^ d.u.b4 ^ d.u.b3 ^ d.u.b0 ^ c.u.b0 ^ c.u.b1 ^ c.u.b2 ^ c.u.b3 ^ c.u.b4 ^ c.u.b5);
    r.u.b4 = (UINT8)(d.u.b63 ^ d.u.b62 ^ d.u.b61 ^ d.u.b60 ^ d.u.b59 ^ d.u.b58 ^ d.u.b57 ^ d.u.b55 ^ d.u.b52 ^ d.u.b49 ^ d.u.b47 ^ d.u.b45 ^ d.u.b44 ^ d.u.b43 ^ d.u.b41 ^ d.u.b39 ^ d.u.b37 ^ d.u.b34 ^ d.u.b33 ^ d.u.b32 ^ d.u.b28 ^ d.u.b27 ^ d.u.b24 ^ d.u.b22 ^ d.u.b21 ^ d.u.b18 ^ d.u.b17 ^ d.u.b13 ^ d.u.b11 ^ d.u.b10 ^ d.u.b9 ^ d.u.b8 ^ d.u.b6 ^ d.u.b5 ^ d.u.b4 ^ d.u.b1 ^ c.u.b0 ^ c.u.b1 ^ c.u.b2 ^ c.u.b3 ^ c.u.b4 ^ c.u.b5 ^ c.u.b6);
    r.u.b5 = (UINT8)(d.u.b63 ^ d.u.b62 ^ d.u.b59 ^ d.u.b58 ^ d.u.b54 ^ d.u.b52 ^ d.u.b51 ^ d.u.b50 ^ d.u.b49 ^ d.u.b47 ^ d.u.b46 ^ d.u.b45 ^ d.u.b42 ^ d.u.b38 ^ d.u.b33 ^ d.u.b32 ^ d.u.b30 ^ d.u.b28 ^ d.u.b27 ^ d.u.b25 ^ d.u.b21 ^ d.u.b20 ^ d.u.b19 ^ d.u.b18 ^ d.u.b12 ^ d.u.b9 ^ d.u.b8 ^ d.u.b6 ^ d.u.b5 ^ d.u.b0 ^ c.u.b1 ^ c.u.b2 ^ c.u.b5 ^ c.u.b6);
    r.u.b6 = (UINT8)(d.u.b63 ^ d.u.b60 ^ d.u.b59 ^ d.u.b55 ^ d.u.b53 ^ d.u.b52 ^ d.u.b51 ^ d.u.b50 ^ d.u.b48 ^ d.u.b47 ^ d.u.b46 ^ d.u.b43 ^ d.u.b39 ^ d.u.b34 ^ d.u.b33 ^ d.u.b31 ^ d.u.b29 ^ d.u.b28 ^ d.u.b26 ^ d.u.b22 ^ d.u.b21 ^ d.u.b20 ^ d.u.b19 ^ d.u.b13 ^ d.u.b10 ^ d.u.b9 ^ d.u.b7 ^ d.u.b6 ^ d.u.b1 ^ c.u.b2 ^ c.u.b3 ^ c.u.b6);
    return r.value;
}

void Utils_DoEmptyLoop(int count)
{
    while (count--)
    {
    }
}

void Utils_BubleSortInt(int* arr, int n)
{
    int i, j, temp;
    for (j = 0; j < n - 1; j++)
    {
        for (i = 0; i < n - j - 1; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
            }
        }
    }
}

int Utils_UniqueInt(int* arr, int n)
{
    int i, j, k;
    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (arr[j] == arr[i])
            {
                for (k = j + 1; k < n; k++)
                {
                    arr[k - 1] = arr[k];
                }
                n--;
            }
        }
    }

    return n;
}

int Utils_FindDev(const int vendorId, const int deviceId, DevBaseInfo* info, const int infoCnt)
{
    int busNo         = 0;
    int deviceNo      = 0;
    int funcNo        = 0;
    UINT32 membaseCsr = 0;
    UINT8 irq         = 0;
    int i             = 0;
    int devCnt        = 0;
    STATUS ret        = ERROR;

    for (i = 0; i < infoCnt; ++i)
    {
#ifdef ACORES653
        ret = pcieFindDevice(PCIE_IFNUM, vendorId, deviceId, i, &busNo, &deviceNo, &funcNo);
#else
        ret = pciFindDevice(vendorId, deviceId, i, &busNo, &deviceNo, &funcNo);
#endif

        if (OK == ret)
        {
#ifdef ACORES653
            pcieConfigInLong(PCIE_IFNUM, busNo, deviceNo, funcNo, PCI_CFG_BASE_ADDRESS_0, &membaseCsr);
            pcieConfigInByte(PCIE_IFNUM, busNo, deviceNo, funcNo, PCI_CFG_DEV_INT_LINE, &irq);
#else
            pciConfigInLong(busNo, deviceNo, funcNo, PCI_CFG_BASE_ADDRESS_0, &membaseCsr);
            pciConfigInByte(busNo, deviceNo, funcNo, PCI_CFG_DEV_INT_LINE, &irq);
#endif

            info[i].busNo = busNo;
            info[i].deviceNo = deviceNo;
            info[i].funcNo = busNo;
            info[i].membaseCsr = membaseCsr;
            info[i].irq = irq;
            devCnt++;
        }
    }
    return devCnt;
}
