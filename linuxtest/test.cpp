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
