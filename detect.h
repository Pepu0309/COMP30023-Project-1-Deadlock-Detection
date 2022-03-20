#ifndef COMP30023_PROJECT_1_DETECT_H
#define COMP30023_PROJECT_1_DETECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define FILENAME_FLAG "-f"
#define EXECUTION_TIME_FLAG "-e"
#define PROCESS_ALLOCATION_FLAG "-c"

#include "hashtable.h"

void parseResourceFile(char *filename, hashTableBucket_t hashTable[], int *numProcess,  int *numFiles);

#endif //COMP30023_PROJECT_1_DETECT_H