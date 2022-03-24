#include "process_allocation.h"

/* Takes a file pointer of the input resource file, parses it and sets up the necessary data structures for process
 * allocation and then calls some other functions in order to complete process allocation. This function does not
 * utilise a RAG as the problem is defined differently and behaves differently from Tasks 2-5. Instead, a much more
 * simplified struct called process_t is used (which can be found in process_allocation.h) in a dynamic array. */
void allocateProcesses(FILE *fp) {

    /* Initialise 2 dynamic arrays. One which stores all the current locked files at t simulation time tick, and
     * another one which stores all the processes that needs to be executed as per the input resource file. */
    uint32_t numLockedFiles = 0, curMaxNumLockedFiles = DYNAMIC_ARRAY_INITIAL_STORAGE;
    uint32_t *curLockedFiles = (uint32_t *) malloc (curMaxNumLockedFiles * sizeof(uint32_t));
    assert(curLockedFiles != NULL);

    uint32_t numProcesses = 0, curMaxNumProcesses = DYNAMIC_ARRAY_INITIAL_STORAGE;
    process_t *processesArray = (process_t *) malloc (curMaxNumProcesses * sizeof(process_t));
    assert(processesArray != NULL);

    uint32_t processID, fileRequestedID1, fileRequestedID2;

    /* Read in the processes and files in the prescribed format and creates a process_t struct for each process and
     * it's 2 files needed. Details of the process struct can be found in the process_allocation.h header file. */
    while(fscanf(fp, "%"PRIu32" %"PRIu32" %"PRIu32"", &processID, &fileRequestedID1, &fileRequestedID2) == 3) {
        createProcess(&processesArray, &numProcesses, &curMaxNumProcesses,
                      processID, fileRequestedID1, fileRequestedID2);
    }

    /* Initialise some variables that need to be tracked during the simulation. */
    uint32_t numProcessesFinished = 0;
    uint32_t simulationTimeTick = 0;
    process_t *curProcess;

    /* As described in further detail below, a process is considered finished when it has been allocated both the
     * files it needs. Hence, the while loop keeps running until all processes get their files they requested.
     * Each iteration of the while loop represents one tick of simulation time. Within each iteration, the program
     * goes through all the processes in the array, checks whether they have been allocated their files, and assigns
     * them their respective files that are not already locked by another process in the same tick of simulation
     * if they have not finished executing. If a process has finished executing, it is simply ignored. */
    while(numProcessesFinished != numProcesses) {
        for(uint32_t i = 0; i < numProcesses; i++) {
            curProcess = &(processesArray[i]);
            if(curProcess->filesAllocated == false) {

                /* Check if both files are unlocked. If they are, then the process gets both files. If one
                 * of the files are locked, then move on to the next process. */
                if (inLockedFilesArray(curLockedFiles, curProcess->fileRequestedID1, numLockedFiles) == false &&
                    inLockedFilesArray(curLockedFiles, curProcess->fileRequestedID2, numLockedFiles) == false) {

                    /* Print the output according to the format described by the specification using the PRIu32
                     * macro for the uint32_t format specifier. */
                    printf("%"PRIu32" %"PRIu32" %"PRIu32",%"PRIu32"\n", simulationTimeTick, curProcess->processID,
                           curProcess->fileRequestedID1, curProcess->fileRequestedID2);

                    /* Check if there's space for 2 more locked file IDs so we don't have to check again after
                     * adding one file ID to the array. */
                    reallocCheckUnsignedIntArray(&curLockedFiles, (numLockedFiles + 2), &curMaxNumLockedFiles);

                    /* This process is assigned the files that it needs and those files are locked for other
                     * processes. This file is then considered to be done in terms of allocation, though it will
                     * take 1 more time unit to actually finish and release the files (though those files can be
                     * immediately accessed by another process again at the same time unit). The way
                     * the simulationTimeTick variable is setup to increment accounts for this by adding
                     * time units at the end. Even if all files are considered finished in terms of allocation,
                     * the program adds one more simulationTimeTick at the very end for the last process(es) to
                     * be finished with their files and release them. */
                    curLockedFiles[numLockedFiles] = curProcess->fileRequestedID1;
                    numLockedFiles++;

                    curLockedFiles[numLockedFiles] = curProcess->fileRequestedID2;
                    numLockedFiles++;

                    curProcess->filesAllocated = true;
                    numProcessesFinished++;

                }
            }
        }
        /* "Wipe" the elements in the curLockedFiles array by setting all the locked file IDs in this time tick to a
         * sentinel value UINT32_MAX and setting numLockedFiles to 0. Setting numLockedFiles = 0 should be sufficient
         * since the inLockedFilesArray function won't be checking those spaces in the array but setting
         * everything to a sentinel value improves the safety of the program. */
        for(uint32_t i = 0; i < numLockedFiles; i++) {
            curLockedFiles[i] = UINT32_MAX;
        }
        numLockedFiles = 0;

        simulationTimeTick++;
    }

    /* Simulation is done now so print the total simulation time and free all the memory used. */
    printf("Simulation time %"PRIu32"\n", simulationTimeTick);

    free(curLockedFiles);
    free(processesArray);
}

/* Creates a process_t variable using the parameters defined in the process allocation input resource file in the
 * dynamic array. */
void createProcess(process_t **processArray, uint32_t *numProcesses, uint32_t *curMaxNumProcesses,
                   uint32_t processID, uint32_t fileRequestedID1, uint32_t fileRequestedID2) {

    /* Standard realloc check for the dynamic array before creating a new process_t variable in it. */
    if(*numProcesses >= *curMaxNumProcesses) {
        *curMaxNumProcesses *= DOUBLE_CUR_SPACE;
        *processArray= (process_t *) realloc (*processArray, (*curMaxNumProcesses) * sizeof(process_t));
        assert(processArray != NULL);
    }

    process_t *curProcess = &((*processArray)[*numProcesses]);
    curProcess->processID = processID;
    curProcess->fileRequestedID1 = fileRequestedID1;
    curProcess->fileRequestedID2 = fileRequestedID2;
    curProcess->filesAllocated = false;

    (*numProcesses)++;
}

/* Iterates through the curLockedFiles array which stores all the currently locked files at any given simulation
 * time tick t and finds whether lockedFileIDToSearch is one of the currently locked files. If it is, return true;
 * otherwise, return false. */
bool inLockedFilesArray(uint32_t *curLockedFiles, uint32_t lockedFileIDToSearch, uint32_t numLockedFiles) {
    for(uint32_t i = 0; i < numLockedFiles; i++) {
        if(curLockedFiles[i] == lockedFileIDToSearch) {
            return true;
        }
    }
    return false;
}
