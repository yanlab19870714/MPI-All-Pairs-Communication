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

//Acknowledgements: this code is implemented by referencing pregel-mpi (https://code.google.com/p/pregel-mpi/) by Chuntao Hong.

#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "timer.h"
#include "global.h"
using namespace std;


class Communicator{
public:
	thread main_thread;
	char s1_buf[BUF_SIZE];
	char r1_buf[BUF_SIZE];
	char* s2_buf;
	char* r2_buf;

	//============================================
	void all_to_all(char* s_buf, char* r_buf, int size){
	    int np = get_num_workers();
	    int me = get_worker_id();
		for (int i = 0; i < np; i++)
		{
			int partner = (i - me + np) % np;
			if (me != partner)
			{
				if (me < partner)
				{
					MPI_Send(s_buf, size, MPI_CHAR, partner, COMM_CHANNEL, MPI_COMM_WORLD);
					//receive
					MPI_Recv(r_buf, size, MPI_CHAR, partner, COMM_CHANNEL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
				else
				{
					//receive
					MPI_Recv(r_buf, size, MPI_CHAR, partner, COMM_CHANNEL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Send(s_buf, size, MPI_CHAR, partner, COMM_CHANNEL, MPI_COMM_WORLD);
				}
			}
		}
	}
    
	void run()
	{
	    while(true){
	    	all_to_all(s1_buf, r1_buf, BUF_SIZE);
	    	all_to_all(s2_buf, r2_buf, fold*BUF_SIZE);
	    	task_num += get_num_workers()*fold*BUF_SIZE;
	    }
	}

	Communicator(){
		s2_buf = new char[fold*BUF_SIZE];
		r2_buf = new char[fold*BUF_SIZE];
		main_thread = thread(&Communicator::run, this);
	}

	~Communicator(){
		delete[] s2_buf;
		delete[] r2_buf;
		main_thread.join();
	}
};

#endif
