/*********************************
**********************************
*****create by patrick at 2015/12/1***
**********************************
**********************************/

#ifndef THREAD_H_H_H
#define THREAD_H_H_H

#include <common.h>

#ifdef CUR_OS_LINUX

#undef LEAF_EXPORT
#define LEAF_EXPORT

#else

#ifndef LEAF_EXPORT
#define LEAF_EXPORT _declspec(dllimport)
#pragma comment(lib, "driver.lib")
#endif

#endif

LEAF_EXPORT int32_t add(int32_t a, int32_t b);

#endif
