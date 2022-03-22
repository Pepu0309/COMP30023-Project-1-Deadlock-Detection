#include "process_allocation.h"

void allocateProcesses(FILE *fp) {

    uint32_t numLockedFiles = 0, curMaxNumLockedFiles = DYNAMIC_ARRAY_INITIAL_STORAGE;
    uint32_t *curLockedFiles = (uint32_t *) malloc (curMaxNumLockedFiles * sizeof(uint32_t));
    assert(curLockedFiles != NULL);

    uint32_t numProcesses = 0, curMaxNumProcesses = DYNAMIC_ARRAY_INITIAL_STORAGE;
    process_t *processesArray = (process_t *) malloc (curMaxNumProcesses * sizeof(process_t));

    uint32_t processID, fileRequestedID1, fileRequestedID2;

    while(fscanf(fp, "%u %u %u", &processID, &fileRequestedID1, &fileRequestedID2) == 3) {
        createProcess(&processesArray, &numProcesses, &curMaxNumProcesses,
                      processID, fileRequestedID1, fileRequestedID2);
    }

    uint32_t numFilesFinished = 0;
    uint32_t executionTimeTick = 0;
    process_t *curProcess;
    while(numFilesFinished != numProcesses) {
        numFilesFinished = 0;
        for(uint32_t i = 0; i < numProcesses; i++) {
            curProcess = &(processesArray[i]);
            if(curProcess->finishedExecuting == false) {

                /* Check if both files are unlocked. If they are, then the process gets both files. If one
                 * of the files are locked, then move on to the next process. */
                if (inLockedFilesArray(curLockedFiles, curProcess->fileRequestedID1, numLockedFiles) == false &&
                    inLockedFilesArray(curLockedFiles, curProcess->fileRequestedID2, numLockedFiles) == false) {

                    printf("%u %u %u,%u\n", executionTimeTick, curProcess->processID,
                           curProcess->fileRequestedID1, curProcess->fileRequestedID2);

                    curLockedFiles[numLockedFiles] = curProcess->fileRequestedID1;
                    numLockedFiles++;

                    curLockedFiles[numLockedFiles] = curProcess->fileRequestedID2;
                    numLockedFiles++;

                    curProcess->finishedExecuting = true;
                    numFilesFinished++;

                }
                /* If process is finished executing, add it to the number of files finished. */
            } else {
                numFilesFinished++;
            }
        }
        for(int i = 0; i < numLockedFiles; i++) {
            curLockedFiles[i] = 1000;
        }
        numLockedFiles = 0;

        executionTimeTick++;
    }

}

void createProcess(process_t **processArray, uint32_t *numProcesses, uint32_t *maxNumProcesses,
                   uint32_t processID, uint32_t fileRequestedID1, uint32_t fileRequestedID2) {
    /* ADD REALLOC CHECK LATER */

    //process_t curProcess = (*processArray)[*numProcesses];
    (*processArray)[*numProcesses].processID = processID;
    (*processArray)[*numProcesses].fileRequestedID1 = fileRequestedID1;
    (*processArray)[*numProcesses].fileRequestedID2 = fileRequestedID2;
    (*processArray)[*numProcesses].finishedExecuting = false;

    (*numProcesses)++;
}

bool inLockedFilesArray(uint32_t *curLockedFiles, uint32_t processIDToSearch, int numLockedFiles) {
    for(int i = 0; i < numLockedFiles; i++) {
        if(curLockedFiles[i] == processIDToSearch) {
            return true;
        }
    }
    return false;
}

void removeProcess(process_t **processArray, uint32_t indexToRemove, uint32_t *numProcesses) {

    for(uint32_t i = indexToRemove; i < *numProcesses; i++) {
        (*processArray)[i] = (*processArray)[i+1];
    }
}