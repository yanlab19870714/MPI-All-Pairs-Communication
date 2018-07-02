This program is used to test all-to-all style balanced communication
* all-to-all requesting
* all-to-all responding
* alternating the above 2 steps

The thread that performs all-to-all communication is in Communicator.h

Parameters in global.h that you can tune:
* BUF_SIZE:  size of the request
* fold:  response-size = fold * BUF_SIZE
* PROGRESS_SYNC_TIME_GAP:  frequency of progress report, unit = us
