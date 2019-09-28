#pragma once
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#ifdef __cplusplus
extern "C" {
#endif

#define Ba_Htons htons
#define Ba_Ntohs ntohs

#define Ba_Htonl htonl
#define Ba_Ntohl ntohl

#ifdef __cplusplus
}
#endif
