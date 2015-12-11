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
    IRunable() {}
    virtual ~IRunable() {}
    virtual void run() = 0;
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
