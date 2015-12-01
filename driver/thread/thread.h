#ifndef THREAD_H_H_H
#define THREAD_H_H_H

#include <common.h>

#ifdef CUR_OS_WINDOWS
_declspec(dllexport) int32_t add(int32_t a, int32_t b);
#endif

#endif
