/*********************************
**********************************
*****create by patrick at 2015/12/1***
**********************************
**********************************/

#include "common.h"
#ifdef CUR_OS_LINUX
#undef LEAF_EXPORT
#else
#define LEAF_EXPORT _declspec(dllexport)
#endif

#include "thread.h"

int32_t add(int32_t a, int32_t b) {
    return a + b;
}
