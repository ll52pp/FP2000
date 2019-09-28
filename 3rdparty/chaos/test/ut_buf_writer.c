#include "chaos/cs_buf_writer.h"

void Cs_TestBufWriter(void **state)
{
    char buf[512] = { 0 };
    char* buf2 = NULL;
    char intb[5] = { 0x78, 0x56, 0x34, 0x12, 0x0a };
    int offset = -1;
    int size = -1;
    CsBufWriterContext ctx;

    // attach buf
    Cs_BufWriterAttach(&ctx, buf, 512);

    Cs_BufWriterPush(&ctx, int, 0x12345678); // 写入一个int
    Cs_BufWriterPush(&ctx, char, 0xab); // 继续向后写入一个char
    Cs_BufWriterPushStr(&ctx, intb, 5); // 继续向后写入一个string

    offset = Cs_BufWriterGetOffset(&ctx); // 获取当前写入到哪了, 偏移字节数
    assert(10 == offset);

    buf2 = Cs_BufWriterGetBuf(&ctx, &size); // 获取上下文中Attach的buf地址
    assert(512 == size);
    assert(buf == buf2);
}
