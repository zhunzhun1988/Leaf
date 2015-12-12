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
	MyRunItem() {}
	~MyRunItem() {}
	void run() {
		//Thread::sleep(10*1000*1000);
		printf("run\n");
	}
};

int32_t main() {
  MyRunItem myRun;
  Thread t(&myRun);
  Thread t2(&myRun);
  t.start();
  t2.start();
  Thread::sleep(10*1000*1000);
  
  return 1;
}
