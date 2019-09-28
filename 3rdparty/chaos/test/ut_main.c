#include "ut_header.h"

void main()
{
    const UnitTest tests[] = {
        unit_test(Cs_TestLinkedHashMap),
        unit_test(Cs_TestList),
        unit_test(Cs_TestDList),
        unit_test(Cs_TestRingBuf),
        unit_test(Cs_TestBufSpliter),
        unit_test(Cs_TestFastBufSpliter),
        unit_test(Cs_TestBufWriter),
        unit_test(Cs_TestPingPongBuf),
        unit_test(Cs_TestAlgorithm),
    };

    run_tests(tests);
}
