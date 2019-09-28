#include "chaos/cs_fast_buf_splitter.h"

void Cs_TestFastBufSpliter(void **state)
{
    CsFastBufSpliterContext ctx;
    char* buf = (char*)malloc(256);
    unsigned int* n = NULL;
    double* d = NULL;

    // attach buf
    Cs_FastBufSplitterAttach(&ctx, buf, 256);

    // ��һ��unsighed int����
    n = (unsigned int*)Cs_FastBufSplitterPop(&ctx, sizeof(unsigned int));
    *n = 0x12345678;

    // ����һ��������λ��, �������������һ��double����
    d = (double*)Cs_FastBufSplitterPop(&ctx, sizeof(double));
    *d = 1.23456789;

    // ����attach buf
    Cs_FastBufSplitterAttach(&ctx, buf, 256);
    n = (unsigned int*)Cs_FastBufSplitterPop(&ctx, sizeof(unsigned int));
    d = (double*)Cs_FastBufSplitterPop(&ctx, sizeof(double));
    assert(*n == 0x12345678);
    assert(*d == 1.23456789);
}
