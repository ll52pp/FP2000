#include "ut_header.h"

void main()
{
    const UnitTest tests[] = {
        unit_test(Cs_TestMutex),
        unit_test(Cs_TestRegister),
        unit_test(Cs_TestTask),
        unit_test(Cs_TestInet),
    };

    run_tests(tests);
}
