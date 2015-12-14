/*********************************
**********************************
*****create by patrick at 2015/12/1***
**********************************
**********************************/

#include <thread/thread.h>
#include <common.h>

#include "stdio.h"

class MyRunItem: public IRunable {
public:
	MyRunItem() {mCount = 1000;}
	~MyRunItem() {}
	void run() {
		while (mCount >0 ) {
			{
				Mutex::AutoLock l_(&mMutex);
				if (mCount > 0) {
					printf("%d useed %d\n", Thread::gettid(), mCount--);
				}
			}
			//Thread::sleep(100*1000);
		}
	}
private:
	int32_t mCount;
	Mutex mMutex;
};

int32_t main() {
  MyRunItem myRun;
  Thread t(&myRun);
  Thread t2(&myRun);
  t.start();
  t2.start();
  Thread::sleep(30*1000);
  return 1;
}

/*
Cross Reference: LtThreads.h
xref: /cos-master-app/sdk/app/CommonLibs/LTUtility/1/0/include/utility/LtThreads.h

    Home
    History
    Annotate
    Line#
    Navigate
    Raw
    Download
    only in /cos-master-app/sdk/app/CommonLibs/LTUtility/1/0/include/utility/

1// Copyright (c) 2012. All rights reserved.
2#pragma once
3
4#include <stdint.h>
5#include <sys/types.h>
6#include <time.h>
7#include <pthread.h>
8#include <cassert>
9

16class LtMutex {
17  public:
18    enum {
19        PRIVATE = 0,
20        SHARED = 1
21    };
22
23    LtMutex();
24    explicit LtMutex(const char* name);
25    LtMutex(int32_t type, const char* name = NULL);
26    LtMutex(bool recursive, int32_t type = 0, const char* name = NULL);
27    ~LtMutex();
28
29    // lock or unlock the mutex
30    int32_t lock();
31    int32_t lockTimeout(unsigned msecs);
32    void unlock();
33
34    // lock if possible; returns 0 on success, error otherwise
35    int32_t tryLock();
36
37    // Manages the mutex automatically. It'll be locked when LtAutolock is
38    // constructed and released when LtAutolock goes out of scope.
39    class LtAutolock {
40      public:
41        inline explicit LtAutolock(LtMutex* pMutex) : mLock(*pMutex) { mStatus = mLock.lock(); }
42        inline explicit LtAutolock(LtMutex* pMutex, unsigned msec) : mLock(*pMutex), mMsec(msec)  {
43            msec ? (mStatus = mLock.lockTimeout(msec)) : (mStatus = mLock.lock()); }
44        inline ~LtAutolock() { mLock.unlock(); }
45        int32_t getStatus() { return mStatus; }
46
47      private:
48        LtMutex& mLock;
49        unsigned mMsec;
50        int32_t mStatus;
51    };
52
53  private:
54    friend class LtCondition;
55
56    // A mutex cannot be copied
57    LtMutex(const LtMutex&);
58    LtMutex& operator = (const LtMutex&);
59
60    pthread_mutex_t mMutex;
61};
62
63inline LtMutex::LtMutex() {
64    pthread_mutex_init(&mMutex, NULL);
65}
66
67inline LtMutex::LtMutex(const char* name) {
68    (void)name;
69    pthread_mutex_init(&mMutex, NULL);
70}
71
72inline LtMutex::LtMutex(int32_t type, const char* name) {
73    (void)name;
74    if (type == SHARED) {
75        pthread_mutexattr_t attr;
76        pthread_mutexattr_init(&attr);
77        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
78        pthread_mutex_init(&mMutex, &attr);
79        pthread_mutexattr_destroy(&attr);
80    } else {
81        pthread_mutex_init(&mMutex, NULL);
82    }
83}
84
85inline LtMutex::LtMutex(bool recursive, int32_t type, const char* name) {
86    (void)name;
87    if (type == SHARED) {
88        pthread_mutexattr_t attr;
89        pthread_mutexattr_init(&attr);
90        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
91        if (recursive)
92            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
93        pthread_mutex_init(&mMutex, &attr);
94        pthread_mutexattr_destroy(&attr);
95    } else {
96        if (recursive) {
97            pthread_mutexattr_t attr;
98            pthread_mutexattr_init(&attr);
99            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
100            pthread_mutex_init(&mMutex, &attr);
101            pthread_mutexattr_destroy(&attr);
102        } else {
103            pthread_mutex_init(&mMutex, NULL);
104        }
105    }
106}
107
108inline LtMutex::~LtMutex() {
109    pthread_mutex_destroy(&mMutex);
110}
111
112inline int32_t LtMutex::lock() {
113    return -pthread_mutex_lock(&mMutex);
114}
115
116inline int32_t LtMutex::lockTimeout(unsigned msec) {

121    return -pthread_mutex_lock_timeout_np(&mMutex, msec);
123}
124
125inline void LtMutex::unlock() {
126    pthread_mutex_unlock(&mMutex);
127}
128
129inline int32_t LtMutex::tryLock() {
130    return -pthread_mutex_trylock(&mMutex);
131}
132
138
139typedef LtMutex::LtAutolock LtAutoMutex;
140
149class LtRWLock {
150  public:
151    enum {
152        PRIVATE = 0,
153        SHARED = 1
154    };
155
156    LtRWLock();
157    explicit LtRWLock(const char* name);
158    LtRWLock(int32_t type, const char* name = NULL);
159    ~LtRWLock();
160
161    int32_t readLock();
162    int32_t tryReadLock();
163    int32_t writeLock();
164    int32_t tryWriteLock();
165    void unlock();
166
167    class LtAutoRLock {
168      public:
169        inline explicit LtAutoRLock(LtRWLock* pRWlock) : mLock(*pRWlock)  { mLock.readLock(); }
170        inline ~LtAutoRLock() { mLock.unlock(); }
171      private:
172        LtRWLock& mLock;
173    };
174
175    class LtAutoWLock {
176      public:
177        inline explicit LtAutoWLock(LtRWLock* pRWlock) : mLock(*pRWlock)  { mLock.writeLock(); }
178        inline ~LtAutoWLock() { mLock.unlock(); }
179      private:
180        LtRWLock& mLock;
181    };
182
183  private:
184    // A LtRWLock cannot be copied
185    LtRWLock(const LtRWLock&);
186    LtRWLock& operator = (const LtRWLock&);
187
188    pthread_rwlock_t mRWLock;
189};
190
191inline LtRWLock::LtRWLock() {
192    pthread_rwlock_init(&mRWLock, NULL);
193}
194
195inline LtRWLock::LtRWLock(const char* name) {
196    pthread_rwlock_init(&mRWLock, NULL);
197}
198
199inline LtRWLock::LtRWLock(int32_t type, const char* name) {
200    if (type == SHARED) {
201        pthread_rwlockattr_t attr;
202        pthread_rwlockattr_init(&attr);
203        pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
204        pthread_rwlock_init(&mRWLock, &attr);
205        pthread_rwlockattr_destroy(&attr);
206    } else {
207        pthread_rwlock_init(&mRWLock, NULL);
208    }
209}
210
211inline LtRWLock::~LtRWLock() {
212    pthread_rwlock_destroy(&mRWLock);
213}
214
215inline int32_t LtRWLock::readLock() {
216    return -pthread_rwlock_rdlock(&mRWLock);
217}
218
219inline int32_t LtRWLock::tryReadLock() {
220    return -pthread_rwlock_tryrdlock(&mRWLock);
221}
222
223inline int32_t LtRWLock::writeLock() {
224    return -pthread_rwlock_wrlock(&mRWLock);
225}
226
227inline int32_t LtRWLock::tryWriteLock() {
228    return -pthread_rwlock_trywrlock(&mRWLock);
229}
230
231inline void LtRWLock::unlock() {
232    pthread_rwlock_unlock(&mRWLock);
233}
234
236

245class LtCondition {
246  public:
247    enum {
248        PRIVATE = 0,
249        SHARED = 1
250    };
251
252    LtCondition();
253    explicit LtCondition(int32_t type);
254    ~LtCondition();
255    // Wait on the condition variable.  Lock the mutex before calling.
256    int32_t wait(LtMutex* pMutex);
257    // same with relative timeout
258    int32_t waitRelative(LtMutex* pMutex, int64_t reltime);
259    // Signal the condition variable, allowing one thread to continue.
260    void signal();
261    // Signal the condition variable, allowing all threads to continue.
262    void broadcast();
263
264  private:
265    pthread_cond_t mCond;
266};
267
268inline LtCondition::LtCondition() {
269    pthread_cond_init(&mCond, NULL);
270}
271inline LtCondition::LtCondition(int32_t type) {
272    if (type == SHARED) {
273        pthread_condattr_t attr;
274        pthread_condattr_init(&attr);
275        pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
276        pthread_cond_init(&mCond, &attr);
277        pthread_condattr_destroy(&attr);
278    } else {
279        pthread_cond_init(&mCond, NULL);
280    }
281}
282
283inline LtCondition::~LtCondition() {
284    pthread_cond_destroy(&mCond);
285}
286
287inline int32_t LtCondition::wait(LtMutex* pMutex) {
288    assert(pMutex != NULL);
289    return -pthread_cond_wait(&mCond, &(pMutex->mMutex));
290}
291
292inline int32_t LtCondition::waitRelative(LtMutex* pMutex, int64_t reltime) {
293    assert(pMutex != NULL);
294// #if defined(HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE)
295    struct timespec ts;
296    ts.tv_sec  = reltime/1000000000;
297    ts.tv_nsec = reltime%1000000000;
298    return -pthread_cond_timedwait_relative_np(&mCond, &(pMutex->mMutex), &ts);
318}
319
320inline void LtCondition::signal() {
321    pthread_cond_signal(&mCond);
322}
323
324inline void LtCondition::broadcast() {
325    pthread_cond_broadcast(&mCond);
326}
327
328
*/
