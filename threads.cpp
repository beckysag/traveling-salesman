
#include "threads.h"


void Thread::start() {
	int r;
	//this->arg = arg;
	if ((r = pthread_create(&_id, NULL, &Thread::exec, this)) != 0) {
		cout << strerror(r) << endl;
		throw "Error";
	}
}



void Thread::join() {
	///////////////////////////////////////////////////
	// Allow thread to wait for the termination status
	///////////////////////////////////////////////////
	void *status;
	int r = pthread_join(_id, &status);
	if (r) {
		printf("ERROR in thread %d; return code from pthread_join() is %d\n",my_id, r);
		exit(-1);
	}
	//if (DEBUG) printf("Joined with thread %ld, status of %ld\n", my_id, (long) status);
}



void *Thread::exec(void *thr) {
	///////////////////////////////////////////////////
	// Function that is to be executed by the thread
	///////////////////////////////////////////////////
	reinterpret_cast<Thread *>(thr)->run();
	//	((Thread *)thr)->run(); ?? are these the same
	return NULL;
}



/*
void Thread::start(void *arg) {
	int ret;
	this->arg = arg;
	 *
	 * Since pthread_create is a C library function, the 3rd argument is
	 * a global function that will be executed by the thread. In C++, we
	 * emulate the global function using the static member function that
	 * is called exec. The 4th argument is the actual argument passed to
	 * the function exec. Here we use this pointer, which is an instance
	 * of the Thread class.
	 *
	if ((ret = pthread_create(&_id, NULL, &Thread::exec, this)) != 0) {
		cout << strerror(ret) << endl;
		throw "Error";
	}
} */
