//this is the server of managing vcache
#ifndef RESPSERVER_H_
#define RESPSERVER_H_

#include "global.h"
using namespace std;

class RespServer {
public:
	thread main_thread;
	char* buf;

    void run()
    {
    	while(true) //otherwise, thread terminates
    	{
    		int has_msg;
    		MPI_Status status;
    		MPI_Iprobe(MPI_ANY_SOURCE, RESP_CHANNEL, MPI_COMM_WORLD, &has_msg, &status);
    		if(!has_msg) usleep(WAIT_TIME_WHEN_IDLE);
    		else
    		{
    			MPI_Recv(buf, fold*BUF_SIZE, MPI_CHAR, status.MPI_SOURCE, RESP_CHANNEL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    			task_num += fold * BUF_SIZE;
    		}
    	}
    }

    RespServer() //get cache_table from Worker
    {
    	buf = new char[fold*BUF_SIZE];
    	main_thread = thread(&RespServer::run, this);
    }

    ~RespServer()
	{
    	delete[] buf;
    	main_thread.join();
	}
};

#endif
