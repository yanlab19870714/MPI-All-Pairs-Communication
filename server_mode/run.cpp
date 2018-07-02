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

#include <iostream>
#include "RespServer.h"
#include "Requester.h"
#include "ReqServer.h"
#include "global.h"
#include <vector>

using namespace std;

bool first_sync = true;
vector<int> previous;

void progress_sync()
{
	if (_my_rank != MASTER_RANK)
	{//send task_num to master
		int cur = task_num;
		MPI_Send(&cur, 1, MPI_INT, MASTER_RANK, PROGRESS_CHANNEL, MPI_COMM_WORLD);
	}
	else
	{
		vector<int> vec(_num_workers); //collecting task_num from all workers
		size_t total = 0;
		for(int i = 0; i < _num_workers; i++)
		{
			if(i != MASTER_RANK)
			{
				int tmp;
				MPI_Recv(&tmp, 1, MPI_INT, i, PROGRESS_CHANNEL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				vec[i] = tmp;
			}
			else
			{
				vec[i] = task_num;
			}
			total += vec[i];
		}
		//======
		cout<<endl;
		cout<<"============ PROGRESS REPORT ============"<<endl;
		cout<<"Total number of tasks processed: "<<total<<endl;
		for(int i=0; i<_num_workers; i++)
		{
			if(first_sync)
			{
				cout<<"Worker "<<i<<": "<<vec[i]<<" tasks processed"<<endl;
			}
			else
			{
				int delta = vec[i] - previous[i];
				cout<<"Worker "<<i<<": "<<vec[i]<<" tasks processed (+ "<<delta<<")"<<endl;
			}
			previous[i] = vec[i];
		}
		cout<<endl;
	}
	first_sync = false;
}

int main(int argc, char* argv[]) {
	if(argc != 3)
    {
        cout<<"arg1 = req-sleep time (1000000 = 1 second),  arg2 = fold"<<endl;
        return -1;
    }
	REQ_TIME_GAP = atoi(argv[1]);
	fold = atoi(argv[2]);
	int provided;
	MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
	if(provided != MPI_THREAD_MULTIPLE)
	{
		printf("MPI do not Support Multiple thread\n");
		exit(0);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &_num_workers);
	previous.resize(_num_workers);
	MPI_Comm_rank(MPI_COMM_WORLD, &_my_rank);

	Requester rqq;
	ReqServer rqs;
	RespServer rss;
	cout<<_my_rank<<": init-ed"<<endl;

	while(true)
	{
		progress_sync();
		usleep(PROGRESS_SYNC_TIME_GAP);
	}

	MPI_Finalize();
	return 0;
}
