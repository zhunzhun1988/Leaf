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

#include <pthread.h>
typedef pthread_t ThreadHandler;

#else

#ifndef LEAF_EXPORT
#define LEAF_EXPORT _declspec(dllimport)
#pragma comment(lib, "driver.lib")
#endif

typedef int32_t ThreadHandler;
#endif

// start define

LEAF_EXPORT class IRunable {
public:
    explicit IRunable() {}
    virtual ~IRunable() {}
    virtual void run() = 0;
};

LEAF_EXPORT class Mutex {
public:
	enum {
        PRIVATE = 0,
        SHARED = 1
    };
    Mutex();
	~Mutex();
    explicit Mutex(const char* name);
    Mutex(int32_t type, const char* name = NULL);
    Mutex(bool recursive, int32_t type = 0, const char* name = NULL);
	int32_t lock();
	int32_t lockTimeout(uint32_t msecs);
	void unlock();
	int32_t tryLock();
	class AutoLock {
		public:
        inline explicit AutoLock(Mutex* pMutex) : mLock(*pMutex) { mStatus = mLock.lock(); }
        inline explicit AutoLock(Mutex* pMutex, uint32_t msec) : mLock(*pMutex), mMsec(msec)  {
            msec ? (mStatus = mLock.lockTimeout(msec)) : (mStatus = mLock.lock()); }
        inline ~AutoLock() { mLock.unlock(); }
        int32_t getStatus() { return mStatus; }

      private:
        Mutex& mLock;
        uint32_t mMsec;
        int32_t mStatus;
	};
private:
    // A mutex cannot be copied
    //Mutex(const Mutex&);
    //Mutex& operator = (const Mutex&);
#ifdef CUR_OS_LINUX
    pthread_mutex_t mMutex;
#endif
};

LEAF_EXPORT class Thread: public IRunable {
public:
    Thread();
    explicit Thread(IRunable* pRunable);
    virtual ~Thread();
    virtual void run();
    bool start();
    bool stop();

private:
	static void *threadFun(void *arg);
    IRunable* mpRunable;
	ThreadHandler mThreadHandler;
	bool mIsStarted;
};

#endif
