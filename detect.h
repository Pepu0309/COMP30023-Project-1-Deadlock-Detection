#ifndef COMP30023_PROJECT_1_DETECT_H
#define COMP30023_PROJECT_1_DETECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define FILENAME_FLAG "-f"
#define EXECUTION_TIME_FLAG "-e"
#define PROCESS_ALLOCATION_FLAG "-c"

#define ACCESS_TIME_DELAY 1

#define INITIAL_DEADLOCKED_PROCESSES 5

#include "hashtable.h"

void parseResourceFile(char *filename, hashTableBucket_t hashTable[], int *numProcess,  int *numFiles);

int calculateExecutionTime(hashTableBucket_t hashTable[]);

void detectDeadlocks(hashTableBucket_t hashTable[], int **deadlockedProcessIDs, int *numDeadlocks);

void sortProcessIDs(int **deadlockedProcessIDs, int numDeadlocks);

#endif //COMP30023_PROJECT_1_DETECT_H