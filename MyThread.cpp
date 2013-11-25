//==================================================================
// File		: mythread.h
// Author	: rsagalyn
// Date		: Aug 25, 2013
// Description	: Subclass of threads.h, implements run()
//==================================================================

#include "MyThread.h"

void MyThread::run()
{
	int result = mytsp->find_best_path(start_node);

	mytsp->path_vals[my_id][0] = start_node;
	mytsp->path_vals[my_id][1] = result;

	if (DEBUG) cout << "thread " << setw(4) << left << my_id << setw(8) << left
			<< " result: " << setw(5) << left << result << endl;

	pthread_exit(NULL);
}
