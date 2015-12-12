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
#include <stdio.h>


inline Mutex::Mutex() {
#ifdef CUR_OS_LINUX
    pthread_mutex_init(&mMutex, NULL);
#endif
}

inline Mutex::Mutex(const char* name) {
#ifdef CUR_OS_LINUX
    (void)name;
    pthread_mutex_init(&mMutex, NULL);
#endif
}

inline Mutex::Mutex(int32_t type, const char* name) {
#ifdef CUR_OS_LINUX
    (void)name;
    if (type == SHARED) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&mMutex, &attr);
        pthread_mutexattr_destroy(&attr);
    } else {
        pthread_mutex_init(&mMutex, NULL);
    }
#endif
}

inline Mutex::Mutex(bool recursive, int32_t type, const char* name) {
#ifdef CUR_OS_LINUX
    (void)name;
    if (type == SHARED) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        if (recursive)
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&mMutex, &attr);
        pthread_mutexattr_destroy(&attr);
    } else {
        if (recursive) {
            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&mMutex, &attr);
            pthread_mutexattr_destroy(&attr);
        } else {
            pthread_mutex_init(&mMutex, NULL);
        }
    }
#endif
}

inline Mutex::~Mutex() {
#ifdef CUR_OS_LINUX
    pthread_mutex_destroy(&mMutex);
#endif
}

inline int32_t Mutex::lock() {
#ifdef CUR_OS_LINUX
    return -pthread_mutex_lock(&mMutex);
#endif
	return 0;
}

inline int32_t Mutex::lockTimeout(unsigned msec) {
#ifdef CUR_OS_LINUX
    return -pthread_mutex_lock(&mMutex);
#endif
	return 0;
}

inline void Mutex::unlock() {
#ifdef CUR_OS_LINUX
    pthread_mutex_unlock(&mMutex);
#endif
}

inline int32_t Mutex::tryLock() {
#ifdef CUR_OS_LINUX
    return -pthread_mutex_trylock(&mMutex);
#endif
	return 0;
}

void *Thread::threadFun(void *arg) {
	IRunable* pRunable = static_cast<IRunable*>(arg);
	if (pRunable) {
		printf("thread [%d] run start\n", Thread::gettid());
		pRunable->run();
	    printf("thread [%d] run end\n",  Thread::gettid());
	}
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

int32_t Thread::gettid() {
#ifdef CUR_OS_LINUX
	return syscall(SYS_gettid);
#endif
	return -1;
}

void Thread::sleep(int64_t ms) {
#ifdef CUR_OS_LINUX
	usleep(ms);
#endif
}

bool Thread::start() {
	Mutex::AutoLock l_(&mMutex);
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
	Mutex::AutoLock l_(&mMutex);
	return true;
}

void Thread::waitForStop() {
#ifdef CUR_OS_LINUX
	pthread_join(mThreadHandler, NULL);
#endif
}