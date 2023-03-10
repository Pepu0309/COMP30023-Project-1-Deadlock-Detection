#ifndef COMP30023_PROJECT_1_DETECT_H
#define COMP30023_PROJECT_1_DETECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#define FILENAME_FLAG "-f"
#define EXECUTION_TIME_FLAG "-e"
#define PROCESS_ALLOCATION_FLAG "-c"

#define ACCESS_TIME_DELAY 1

#include "hashtable.h"
#include "array.h"
#include "process_allocation.h"

void createResourceAllocationGraph(FILE *fp, hashTableBucket_t hashTable[], uint32_t *numProcess,
                                   uint32_t *numFiles);

uint32_t calculateExecutionTime(hashTableBucket_t hashTable[]);

void handleDeadlocks(hashTableBucket_t hashTable[]);

void detectDeadlocks(hashTableBucket_t hashTable[], uint32_t **deadlockedProcessIDs, uint32_t *numDeadlocks,
                     uint32_t *curMaxNumProcessIDs);

void sortProcessIDs(uint32_t **deadlockedProcessIDs, uint32_t numDeadlocks);

#endif //COMP30023_PROJECT_1_DETECT_H