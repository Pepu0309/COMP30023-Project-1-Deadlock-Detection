#include "process_allocation.h"

void allocateProcesses(FILE *fp) {

    uint32_t numLockedFiles = 0, curMaxNumLockedFiles = DYNAMIC_ARRAY_INITIAL_STORAGE;
    uint32_t *curLockedFiles = (uint32_t *) malloc (curMaxNumLockedFiles * sizeof(uint32_t));
    assert(curLockedFiles != NULL);

    uint32_t numProcesses = 0, curMaxNumProcesses = DYNAMIC_ARRAY_INITIAL_STORAGE;
    process_t *processesArray = (process_t *) malloc (curMaxNumProcesses * sizeof(process_t));
    assert(processesArray != NULL);

    uint32_t processID, fileRequestedID1, fileRequestedID2;

    while(fscanf(fp, "%u %u %u", &processID, &fileRequestedID1, &fileRequestedID2) == 3) {
        createProcess(&processesArray, &numProcesses, &curMaxNumProcesses,
                      processID, fileRequestedID1, fileRequestedID2);
    }

    uint32_t numFilesFinished = 0;
    uint32_t executionTimeTick = 0;
    process_t *curProcess;
    while(numFilesFinished != numProcesses) {
        for(uint32_t i = 0; i < numProcesses; i++) {
            curProcess = &(processesArray[i]);
            if(curProcess->finishedExecuting == false) {

                /* Check if both files are unlocked. If they are, then the process gets both files. If one
                 * of the files are locked, then move on to the next process. */
                if (inLockedFilesArray(curLockedFiles, curProcess->fileRequestedID1, numLockedFiles) == false &&
                    inLockedFilesArray(curLockedFiles, curProcess->fileRequestedID2, numLockedFiles) == false) {

                    printf("%u %u %u,%u\n", executionTimeTick, curProcess->processID,
                           curProcess->fileRequestedID1, curProcess->fileRequestedID2);

                    /* Check if there's space for 2 more locked file IDs so we don't have to check again. */
                    reallocCheckUnsignedIntArray(&curLockedFiles, (numLockedFiles + 2), &curMaxNumLockedFiles);

                    /* This process is assigned the files that it needs and those files are locked for other
                     * processes. This file is then considered finished executing, though it will take 1
                     * more time unit to actually finish and release the files (though those files can be
                     * immediately accessed by another process again at the next time unit). The way
                     * the executionTimeTick variable is setup to increment accounts for this by adding
                     * time units at the end, even if all files are "considered finished". */
                    curLockedFiles[numLockedFiles] = curProcess->fileRequestedID1;
                    numLockedFiles++;

                    curLockedFiles[numLockedFiles] = curProcess->fileRequestedID2;
                    numLockedFiles++;

                    curProcess->finishedExecuting = true;
                    numFilesFinished++;

                }
            }
        }
        for(int i = 0; i < numLockedFiles; i++) {
            curLockedFiles[i] = UINT32_MAX;
        }
        numLockedFiles = 0;

        executionTimeTick++;
    }

    /* REMEMBER TO SWITCH TO PRIu32 format  */
    printf("Simulation time %u\n", executionTimeTick);

    free(curLockedFiles);
    free(processesArray);
}

void createProcess(process_t **processArray, uint32_t *numProcesses, uint32_t *curMaxNumProcesses,
                   uint32_t processID, uint32_t fileRequestedID1, uint32_t fileRequestedID2) {
    /* ADD REALLOC CHECK LATER */
    if(*numProcesses >= *curMaxNumProcesses) {
        *curMaxNumProcesses *= 2;
        *processArray= (process_t *) realloc (*processArray, (*curMaxNumProcesses) * sizeof(process_t));
        assert(processArray != NULL);
    }

    //process_t curProcess = (*processArray)[*numProcesses];
    (*processArray)[*numProcesses].processID = processID;
    (*processArray)[*numProcesses].fileRequestedID1 = fileRequestedID1;
    (*processArray)[*numProcesses].fileRequestedID2 = fileRequestedID2;
    (*processArray)[*numProcesses].finishedExecuting = false;

    (*numProcesses)++;
}

bool inLockedFilesArray(uint32_t *curLockedFiles, uint32_t processIDToSearch, uint32_t numLockedFiles) {
    for(int i = 0; i < numLockedFiles; i++) {
        if(curLockedFiles[i] == processIDToSearch) {
            return true;
        }
    }
    return false;
}
