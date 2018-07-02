//########################################################################
//## Copyright 2018 Da Yan http://www.cs.uab.edu/yanda
//##
//## Licensed under the Apache License, Version 2.0 (the "License");
//## you may not use this file except in compliance with the License.
//## You may obtain a copy of the License at
//##
//## //http://www.apache.org/licenses/LICENSE-2.0
//##
//## Unless required by applicable law or agreed to in writing, software
//## distributed under the License is distributed on an "AS IS" BASIS,
//## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//## See the License for the specific language governing permissions and
//## limitations under the License.
//########################################################################

//this is the server of key-value store
#ifndef REQSERVER_H_
#define REQSERVER_H_

#include "global.h"
using namespace std;

class ReqServer {
public:
	thread main_thread;
	char rbuf[BUF_SIZE];
	char* sbuf;

    void run() //called after graph is loaded and local_table is set
    {
    	while(true) //otherwise, thread terminates
    	{
    		int has_msg;
    		MPI_Status status;
    		MPI_Iprobe(MPI_ANY_SOURCE, REQ_CHANNEL, MPI_COMM_WORLD, &has_msg, &status);
    		if(!has_msg) usleep(WAIT_TIME_WHEN_IDLE);
    		else
    		{
    			int src = status.MPI_SOURCE;
    			MPI_Recv(rbuf, BUF_SIZE, MPI_CHAR, src, REQ_CHANNEL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    			MPI_Send(sbuf, fold*BUF_SIZE, MPI_CHAR, src, RESP_CHANNEL, MPI_COMM_WORLD);
    		}
    	}
    }

    ReqServer()
    {
    	sbuf = new char[fold*BUF_SIZE];
    	main_thread = thread(&ReqServer::run, this);
    }

    ~ReqServer()
    {
    	delete[] sbuf;
    	main_thread.join();
    }
};

#endif
