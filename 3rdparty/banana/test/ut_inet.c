#include "ut_header.h"

void Cs_TestInet(void **state)
{
    unsigned long ul = 0x11223344;
    unsigned long ulNet = Ba_Htonl(ul);
    assert_int_equal(0x44332211, ulNet);

    ul = Ba_Ntohl(ulNet);
    assert_int_equal(0x11223344, ul);
}
