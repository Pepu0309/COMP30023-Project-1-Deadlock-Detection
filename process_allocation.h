#ifndef COMP30023_PROJECT_1_PROCESS_ALLOCATION_H
#define COMP30023_PROJECT_1_PROCESS_ALLOCATION_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include "array.h"

/* In process allocation, each process has it's own ID, the ID of both of it's files requested and a boolean
 * variable which helps the allocateProcess function identify whether the process has had it's files that it
 * requested allocated or not. */
typedef struct process{
    uint32_t processID;

    uint32_t fileRequestedID1;
    uint32_t fileRequestedID2;

    bool filesAllocated;
} process_t;

void allocateProcesses(FILE *fp);

void createProcess(process_t **processArray, uint32_t *numProcesses, uint32_t *curMaxNumProcesses,
        uint32_t processID, uint32_t fileIDRequested1, uint32_t fileIDRequested2);

bool inLockedFilesArray(uint32_t *curLockedFiles, uint32_t processIDToSearch, uint32_t numLockedFiles);

#endif //COMP30023_PROJECT_1_PROCESS_ALLOCATION_H
