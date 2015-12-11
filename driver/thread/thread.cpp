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
#include <cstddef>
#include <stdio.h>

void *Thread::threadFun(void *arg) {
	return NULL;
}

Thread::Thread(): mpRunable(NULL), mIsStarted(false) {
}

Thread::Thread(IRunable* pRunable): mpRunable(pRunable), mIsStarted(false) {
}

Thread::~Thread() {
    mpRunable = NULL;
}
void Thread::run() {
}

bool Thread::start() {
	if (false == mIsStarted) {
#ifdef CUR_OS_LINUX
		int32_t ret = pthread_create(&mThreadHandler, NULL, threadFun, (void*)(NULL == mpRunable? this: mpRunable));
		if (0 != ret) {
			printf("pthread_create error %d\n", ret);
			return false;
		}
		return true;
#endif
	} else {
		printf("Thread has started");
	}
	return true;
}

bool Thread::stop() {
	return true;
}