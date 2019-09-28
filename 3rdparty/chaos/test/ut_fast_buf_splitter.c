#include "chaos/cs_fast_buf_splitter.h"

void Cs_TestFastBufSpliter(void **state)
{
    CsFastBufSpliterContext ctx;
    char* buf = (char*)malloc(256);
    unsigned int* n = NULL;
    double* d = NULL;

    // attach buf
    Cs_FastBufSplitterAttach(&ctx, buf, 256);

    // 弹一个unsighed int出来
    n = (unsigned int*)Cs_FastBufSplitterPop(&ctx, sizeof(unsigned int));
    *n = 0x12345678;

    // 继上一个弹出的位置, 继续弹出后面的一个double出来
    d = (double*)Cs_FastBufSplitterPop(&ctx, sizeof(double));
    *d = 1.23456789;

    // 重新attach buf
    Cs_FastBufSplitterAttach(&ctx, buf, 256);
    n = (unsigned int*)Cs_FastBufSplitterPop(&ctx, sizeof(unsigned int));
    d = (double*)Cs_FastBufSplitterPop(&ctx, sizeof(double));
    assert(*n == 0x12345678);
    assert(*d == 1.23456789);
}
