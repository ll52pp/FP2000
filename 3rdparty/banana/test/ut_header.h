#ifndef BA_TEST_H_1EF37FC9_8510_454C_8CBF_E5C7B1B130B0
#define BA_TEST_H_1EF37FC9_8510_454C_8CBF_E5C7B1B130B0

#include <setjmp.h>
#include "cmockery.h"

#include "banana/ba_inet.h"
#include "banana/ba_mutex.h"
#include "banana/ba_task.h"

void Cs_TestMutex(void **state);
void Cs_TestRegister(void **state);
void Cs_TestTask(void **state);
void Cs_TestInet(void **state);

#endif
