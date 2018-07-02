#ifndef REQUESTER_H_
#define REQUESTER_H_

#include "global.h"
using namespace std;

class Requester {
public:

    thread main_thread;
    char buf[BUF_SIZE] = {0};


    void thread_func() //managing requests to tgt_rank
    {
    	if(_my_rank == 1) cout<<_my_rank<<": here!!!"<<endl;
    	while(true) //otherwise, thread terminates
    	{
    		for(int i = 0; i<_num_workers; i++)
    			MPI_Send(buf, BUF_SIZE, MPI_CHAR, i, REQ_CHANNEL, MPI_COMM_WORLD);
    		usleep(REQ_TIME_GAP);
    	}
    }

    Requester()
    {
    	main_thread = thread(&Requester::thread_func, this);
    }

    ~Requester()
    {
    	main_thread.join();
    }

};

#endif
