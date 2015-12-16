/*********************************
**********************************
*****create by patrick at 2015/12/1***
**********************************
**********************************/

#ifndef THREAD_H_H_H
#define THREAD_H_H_H

#include <common.h>
#include <cstddef>

#ifdef CUR_OS_LINUX

#undef LEAF_EXPORT
#define LEAF_EXPORT

#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
typedef pthread_t ThreadHandler;

#else  // windows

#include <process.h>
#include<windows.h>

#ifndef LEAF_EXPORT
#define LEAF_EXPORT _declspec(dllimport)
#pragma comment(lib, "driver.lib")
#endif

typedef HANDLE ThreadHandler;
#endif

// start define

LEAF_EXPORT class IRunable {
public:
    explicit IRunable() {}
    virtual ~IRunable() {}
    virtual void run() = 0;
};

class LEAF_EXPORT Mutex {
  public:
    friend class Condition;
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
#else
    HANDLE mMutex;
#endif
};

class LEAF_EXPORT Condition {
    public:
        enum {
            PRIVATE = 0,
                SHARED = 1
        };
        Condition();
        explicit Condition(int32_t type);
        ~Condition();
        int32_t wait(Mutex* pMutex);
        int32_t waitRelative(Mutex* pMutex, int64_t reltime);
        void signal();
        void signalAll();

     private:
        #ifdef CUR_OS_LINUX
            pthread_cond_t mCond;
        #endif
};

class LEAF_EXPORT Thread: public IRunable {
public:
    Thread();
    explicit Thread(IRunable* pRunable);
    virtual ~Thread();
    virtual void run();
    bool start();
    bool forceStop();
    void waitForStop();
    static void sleep(int64_t ms);
    static int32_t gettid();

private:
       IRunable* mpRunable;
	ThreadHandler mThreadHandler;
	bool mIsStarted;
	Mutex mMutex;
};

#endif
