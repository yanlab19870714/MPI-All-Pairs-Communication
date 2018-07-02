#ifndef GLOBAL_H
#define GLOBAL_H

#include <mpi.h>
#include <unistd.h> //for usleep()
#include <thread>
#include <atomic>
#include <ctime>

#define PROGRESS_SYNC_TIME_GAP 1000000 //report frequency, unit: us
#define BUF_SIZE 10000
#define MASTER_RANK 0
#define PROGRESS_CHANNEL 205
#define COMM_CHANNEL 301

int _my_rank;
int _num_workers;

int fold = 100; //response-size = fold * request-size
std::atomic<int> task_num(0);

inline int get_worker_id()
{
    return _my_rank;
}
inline int get_num_workers()
{
    return _num_workers;
}

#endif
