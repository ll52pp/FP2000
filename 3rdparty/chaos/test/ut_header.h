#pragma once

#include <setjmp.h>
#include "cmockery.h"

void Cs_TestList(void **state);
void Cs_TestDList(void **state);
void Cs_TestRingBuf(void **state);
void Cs_TestBufSpliter(void **state);
void Cs_TestFastBufSpliter(void **state);
void Cs_TestBufWriter(void **state);
void Cs_TestPingPongBuf(void **state);
void Cs_TestLinkedHashMap(void **state);
void Cs_TestAlgorithm(void **state);
