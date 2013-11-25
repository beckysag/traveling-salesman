//==================================================================
// File		: mythread.h
// Author	: rsagalyn
// Date		: Aug 25, 2013
// Description	: Subclass of threads.h, implements run()
//==================================================================
#ifndef SUBTHREAD_H
#define SUBTHREAD_H

#include "threads.h"

using namespace std;

//
class MyThread: public Thread
{
public:

	// This method will be executed by the Thread::exec method,
	// which is executed in the thread of execution
	virtual void run();
};

#endif
