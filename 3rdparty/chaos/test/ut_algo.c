#include "ut_header.h"
#include "chaos/cs_algorithm.h"

void testCrc32()
{
    assert_int_equal(0xF9EB20AD, Cs_Crc32(0, "helloworld", strlen("helloworld")));
    assert_int_equal(0x4C2750BD, Cs_Crc32(0, "abcdefghijklmnopqrstuvwxyz", strlen("abcdefghijklmnopqrstuvwxyz")));
}

void testByteAlign()
{
    assert_int_equal(1024, Cs_CalcByteAlignment(1023, 4));
    assert_int_equal(1024, Cs_CalcByteAlignment(1023, 8));
    assert_int_equal(4096, Cs_CalcByteAlignment(1023, 4096));
}

void Cs_TestAlgorithm(void **state)
{
    testCrc32();
    testByteAlign();
}
