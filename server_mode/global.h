#ifndef GLOBAL_H
#define GLOBAL_H

#include <mpi.h>
#include <unistd.h> //for usleep()
#include <thread>
#include <atomic>
#include <ctime>

int REQ_TIME_GAP;
#define PROGRESS_SYNC_TIME_GAP 1000000
#define BUF_SIZE 100 
#define MASTER_RANK 0
#define REQ_CHANNEL 201
#define RESP_CHANNEL 202
#define PROGRESS_CHANNEL 205
#define WAIT_TIME_WHEN_IDLE 100

int _my_rank;
int _num_workers;

int fold = 25;
std::atomic<int> task_num(0);

#endif
