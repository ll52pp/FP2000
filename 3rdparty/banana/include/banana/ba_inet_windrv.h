#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define Ba_Htons RtlUshortByteSwap
#define Ba_Ntohs RtlUshortByteSwap

#define Ba_Htonl RtlUlongByteSwap
#define Ba_Ntohl RtlUlongByteSwap

#ifdef __cplusplus
}
#endif
