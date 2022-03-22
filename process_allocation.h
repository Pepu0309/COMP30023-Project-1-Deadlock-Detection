#ifndef COMP30023_PROJECT_1_PROCESS_ALLOCATION_H
#define COMP30023_PROJECT_1_PROCESS_ALLOCATION_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include "array.h"

typedef struct process{
    uint32_t processID;

    uint32_t fileRequestedID1;
    uint32_t fileRequestedID2;

    bool finishedExecuting;
} process_t;

void allocateProcesses(FILE *fp);

void createProcess(process_t **processArray, uint32_t *numProcesses, uint32_t *maxNumProcesses,
        uint32_t processID, uint32_t fileIDRequested1, uint32_t fileIDRequested2);

bool inLockedFilesArray(uint32_t *curLockedFiles, uint32_t processIDToSearch, int numLockedFiles);

#endif //COMP30023_PROJECT_1_PROCESS_ALLOCATION_H
