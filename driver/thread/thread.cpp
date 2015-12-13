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
#else
    mMutex = CreateMutex(NULL, false, L"");
#endif
}

inline Mutex::~Mutex() {
#ifdef CUR_OS_LINUX
    pthread_mutex_destroy(&mMutex);
#else
    CloseHandle(mMutex);
#endif
}

inline int32_t Mutex::lock() {
#ifdef CUR_OS_LINUX
    return -pthread_mutex_lock(&mMutex);
#else
    return WaitForSingleObject(mMutex, INFINITE);
#endif
}

inline int32_t Mutex::lockTimeout(unsigned msec) {
#ifdef CUR_OS_LINUX
    return -pthread_mutex_lock(&mMutex);
#else
    int32_t ret = WaitForSingleObject(mMutex, msec);
    if (ret != WAIT_OBJECT_0 || ret != WAIT_TIMEOUT) {
        printf("WaitForSingleObject error %d\n", ret);
        return -1;
    } else {
        return ret == WAIT_OBJECT_0? 0: -1;
    }
#endif
	return 0;
}

inline void Mutex::unlock() {
#ifdef CUR_OS_LINUX
    pthread_mutex_unlock(&mMutex);
#else
    ReleaseMutex(mMutex);
#endif
}

inline int32_t Mutex::tryLock() {
#ifdef CUR_OS_LINUX
    return -pthread_mutex_trylock(&mMutex);
#else
    int32_t ret = WaitForSingleObject(mMutex, 0);
    if (ret != WAIT_OBJECT_0 || ret != WAIT_TIMEOUT) {
        printf("WaitForSingleObject error %d\n", ret);
        return -1;
    } else {
        return ret == WAIT_OBJECT_0? 0: -1;
    }
#endif
}
#ifdef CUR_OS_LINUX
void * threadFun(void *arg) {
   pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);  // set thread can canceled
   pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
#else
uint32_t __stdcall threadFun(void *arg) {
#endif
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
    forceStop();
    waitForStop();
}
void Thread::run() {
}

int32_t Thread::gettid() {
#ifdef CUR_OS_LINUX
    return syscall(SYS_gettid);
#else
    return GetCurrentThreadId();
#endif
}

void Thread::sleep(int64_t ms) {
#ifdef CUR_OS_LINUX
    usleep(ms*1000);
#else
    Sleep(ms);
#endif
}

bool Thread::start() {
    Mutex::AutoLock l_(&mMutex);
    if (false == mIsStarted) {
#ifdef CUR_OS_LINUX
    int32_t ret = pthread_create(&mThreadHandler, NULL, &threadFun, (void*)(NULL == mpRunable? this: mpRunable));
    if (0 != ret) {
        printf("pthread_create error %d\n", ret);
        return false;
    }
    return true;
#else
    mThreadHandler = (ThreadHandler)_beginthreadex(NULL, 0, threadFun, (void*)(NULL == mpRunable? this: mpRunable), 0, NULL);
#endif
    } else {
        printf("Thread has started");
    }
    return true;
}

bool Thread::forceStop() {
    Mutex::AutoLock l_(&mMutex);
#ifdef CUR_OS_LINUX
    return 0 == pthread_cancel(mThreadHandler);
#else
    return TerminateThread((HANDLE)mThreadHandler, 0);
#endif
}

void Thread::waitForStop() {
#ifdef CUR_OS_LINUX
	pthread_join(mThreadHandler, NULL);
#else
	WaitForSingleObject((HANDLE)mThreadHandler, INFINITE);
#endif
}