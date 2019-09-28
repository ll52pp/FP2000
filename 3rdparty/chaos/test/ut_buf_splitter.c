#include "chaos/cs_buf_splitter.h"

typedef struct
{
    int id;
    int age;
} MyTestInfo;

void _csPacketSpliterDoSomething(char* buf, int size)
{
    MyTestInfo* info = (MyTestInfo*)buf;
    assert(NULL != buf);
    assert(sizeof(MyTestInfo) == size);
    info->id = 0x01234567;
    info->age = 0x89abcdef;
}

void Cs_TestBufSpliter(void **state)
{
    const int BUF_SIZE = sizeof(MyTestInfo) * 10;
    char* buf = (char*)malloc(BUF_SIZE);
    char* ptr = NULL;
    int realSize = 0;
    int cnt = 0;

    CsBufSpliterContext ctx;

    // attach buf
    Cs_BufSplitterAttach(&ctx, buf, BUF_SIZE);

    // 切割buf
    cnt = 0;
    ptr = Cs_BufSplitterGet(&ctx, sizeof(MyTestInfo), &realSize);
    while (ptr)
    {
        _csPacketSpliterDoSomething(ptr, realSize);
        cnt++;
        ptr = Cs_BufSplitterGet(&ctx, sizeof(MyTestInfo), &realSize);
    }

    assert(10 == cnt);

    Cs_BufSplitterSeek(&ctx, 0); // 将切割点重新置0
    Cs_BufSplitterSkip(&ctx, sizeof(MyTestInfo) * 9); // 从当前切割点, 向后偏移指定的字节大小
    ptr = Cs_BufSplitterGet(&ctx, sizeof(MyTestInfo), &realSize);
    cnt = 0;
    while (ptr)
    {
        MyTestInfo* info = (MyTestInfo*)ptr;
        assert(0x01234567 == info->id);
        assert(0x89abcdef == info->age);
        cnt++;

        ptr = Cs_BufSplitterGet(&ctx, sizeof(MyTestInfo), &realSize);
    }

    assert(1 == cnt);

    Cs_BufSplitterSeek(&ctx, 4); // 将切割点重新置到第4个字节的位置
    Cs_BufSplitterSkip(&ctx, sizeof(MyTestInfo) * 9); // 从当前切割点, 向后偏移指定的字节大小
    Cs_BufSplitterGet(&ctx, sizeof(MyTestInfo), &realSize);
    assert(4 == realSize);
    ptr = Cs_BufSplitterGet(&ctx, sizeof(MyTestInfo), NULL);
    assert(NULL == ptr);

    free(buf);
}
