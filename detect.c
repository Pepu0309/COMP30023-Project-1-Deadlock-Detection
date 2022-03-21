#include "detect.h"

int main(int argc, char **argv) {

    int filenameFlag = false;
    int executionTimeFlag = false;
    int processAllocationFlag = false;

    /* The number of nodes can be derived by adding numProcess and numFiles. */
    int numProcess = 0;
    int numFiles = 0;

    /* Creates a hash table which is an array of linked lists. Initialise the head and tails of the linked
     * lists to be NULL so they don't point to garbage values. */
    hashTableBucket_t hashTable[NUM_BUCKETS];
    for(int i = 0; i < NUM_BUCKETS; i++) {
        hashTable[i].head = NULL;
        hashTable[i].tail = NULL;
    }

    /* Goes through each argument and identifies which argument type they are. */
    for(int i = 0; i < argc; i++){

        /* If the filename flag is given, then the next argument in the argument vector is the filename for
         * the resource file and that is parsed by calling the parseResourceFile function. */
        if(filenameFlag) {
            parseResourceFile(argv[i], hashTable, &numProcess, &numFiles);
            filenameFlag = false;
        }

        /* Checks which of the 3 flags possible the current argument in argv is. */
        if(strcmp(argv[i], FILENAME_FLAG) == 0) {
            filenameFlag = true;
        } else if (strcmp(argv[i], EXECUTION_TIME_FLAG) == 0){
            executionTimeFlag = true;
        } else if (strcmp(argv[i], PROCESS_ALLOCATION_FLAG) == 0){
            processAllocationFlag = true;
        }
    }

    /* Print statistics for task 1 */
    printf("Processes %d\n", numProcess);
    printf("Files %d\n", numFiles);

    /* If the execution time flag -e was given as an argument, calculate the minimum execution time
     * of the processes; otherwise, detect deadlocks in the processes. */
    if(executionTimeFlag) {
        int minExecutionTime = calculateExecutionTime(hashTable);
        printf("Execution time %d\n", minExecutionTime);
    } else {
        /* Find the deadlocks according to project spec if execution time flag is not given. */
        int numDeadlocks = 0;
        /* Create a dynamic array for storing the smallest process ID of each deadlock (cycle). */
        int *deadlockedProcessIDs = (int *)malloc(sizeof(int) * INITIAL_DEADLOCKED_PROCESSES);
        assert(deadlockedProcessIDs != NULL);

        detectDeadlocks(hashTable, &deadlockedProcessIDs, &numDeadlocks);

        /* If there is 1 deadlock or more, the smallest process ID for each deadlock found is additionally sorted
         * using insertion sort such that the process IDs are displayed in ascending order as indicated by
         * the project spec. */
        if(numDeadlocks >= 1) {
            printf("Deadlock detected\n");
            sortProcessIDs(&deadlockedProcessIDs, numDeadlocks);
            printf("Terminate");
            for(int i = 0; i < numDeadlocks; i++) {
                printf(" %d", deadlockedProcessIDs[i]);
            }
            printf("\n");
        } else {
            printf("No deadlocks\n");
        }
    }

    if(processAllocationFlag) {
        //Challenge task will go here.
    }

    /* Everything is done, free all the nodes in the hash table. */
    freeHashTable(hashTable);

    return 0;
}

/* Parses the resource file and creates all the process and file nodes required and stores them in a hash table.
 * Also creates an implicit adjacency list using the dependencyTo attributes of the nodes. */
void parseResourceFile(char *filename, hashTableBucket_t hashTable[], int *numProcess,  int *numFiles) {

    FILE *fp = fopen(filename, "r");

    unsigned int processID, lockedFileID, requiredFileID;

    RAGNode_t *processNode, *lockedFileNode, *requiredFileNode;

    /* As described by the spec, the format of each line of the input file will always be:
     * processID lockedfileID requiredfileID. Read each line and create all the nodes and link them together
     * according to their dependencies. */
    while(fscanf(fp, "%u %u %u", &processID, &lockedFileID, &requiredFileID) == 3) {

        /* Processes are distinct, so we don't need to search for duplicates. */
        processNode = createNode(hashTable, processID, PROCESS_NODE);
        (*numProcess)++;


        /* For both the file nodes (locked file and required file), we search the hashTable for a duplicate node. If
         * we do find one, then the function will return the node. If we don't find one, the function returns NULL
         * and the program then creates a new node based on the parameters. */
        if((lockedFileNode = searchDuplicate(hashTable, lockedFileID, FILE_NODE)) == NULL) {
            lockedFileNode = createNode(hashTable, lockedFileID, FILE_NODE);
            (*numFiles)++;
        }

        if((requiredFileNode = searchDuplicate(hashTable, requiredFileID, FILE_NODE)) == NULL) {
            requiredFileNode = createNode(hashTable, requiredFileID, FILE_NODE);
            (*numFiles)++;
        }

        /* Now we handle the dependencies of the resource nodes. A process node always has a dependency to the file
         * it requires. The file being required also stores the number of times it is requested in the system to
         * be used for calculating execution time. A locked file node always has a dependency to the process it
         * is locked by. Even if one of the nodes is a duplicate, this behaviour is intended to create the
         * resource allocation graph. */
        processNode->dependencyTo = requiredFileNode;
        (requiredFileNode->numRequests)++;
        lockedFileNode->dependencyTo = processNode;

    }

    fclose(fp);
}


/* The minimum execution time can be given my the formula n+1.

n is the maximum number of times a single given file is requested by a process
in the system. As such, the process will need to take a minimum of n time units
for the most requested file to be accessed by all the processes requesting as one
and only one process will only be able to access that same requested file at any
given time.

Files locked at time = 0 are being finished processed on at time = 1. With that in
mind, only at time = 1 or above can the files being requested be accessed and used
by a process. To finish processing the files requires 1 time unit. For example,
file 1 is requested 3 times (n = 2), first access is at time = 1, it finishes
getting processed at time 2 and at the same  Hence, an extra time unit is added to the
number of times a file is requested . */
int calculateExecutionTime(hashTableBucket_t hashTable[]) {
    int maxNumRequests = 0;
    linkedListNode_t *curLLNode;
    int curNumRequests;

    /* Go through all the nodes to determine the max number of requests of any given node (only files matter in
     * this case but access time to check the type is the same anyway so just check the requests of any node). */
    for(int i = 0; i < NUM_BUCKETS; i++) {
        curLLNode = hashTable[i].head;
        while (curLLNode != NULL) {
            if((curNumRequests = curLLNode->RAGNode->numRequests) > maxNumRequests) {
                maxNumRequests = curNumRequests;
            }
            curLLNode = curLLNode->next;
        }
    }

    /* Add 1 as described above to get the minimum execution time of the system. */
    return maxNumRequests+ACCESS_TIME_DELAY;
}

/* Goes through the hash table and calls the visitNode recursive Depth First Search (DFS) function to find cycles
 * in the resource allocation graph. If a cycle is found, then there is a deadlock. For more details on criteria of
 * a cycle, refer to visitNode function in node.c file. */
void detectDeadlocks(hashTableBucket_t hashTable[], int **deadlockedProcessIDs, int *numDeadlocks) {

    /* Initialise some variables to be used for traversing the hash table */
    linkedListNode_t *curLLNode;
    RAGNode_t *nodeInCycle = NULL;

    /* By default, assume there is no deadlock. We also start at 0 as the current iteration of the DFS call to track
     * which visitNode DFS call a node was visited on. For details of what the iteration of the DFS call is used
     * for, refer to visitNode function at node.c */
    int isDeadlocked = false;
    unsigned int currentIterationOfDFS = 0;
    int processToTerminateID;


    /* Traverse through the hash table and call the recursive visitNode DFS function on any unvisited process nodes.
     * If the visitNode function exists and found a deadlock and a node not in a cycle associated with it, there
     * is a deadlock. */
    for(int i = 0; i < NUM_BUCKETS; i++) {
        curLLNode = hashTable[i].head;

        while (curLLNode != NULL) {
            /* We are only concerned with calling DFS on an unvisited process node in the hash table as a deadlock
             * only happens a deadlock if the file a process is waiting for, is locked by another process. For a
             * thorough explanation, refer to the visitNode function in node.c file. */
            if(curLLNode->RAGNode->visited == false && curLLNode->RAGNode->nodeType == PROCESS_NODE) {
                visitNode(curLLNode->RAGNode, &isDeadlocked, &nodeInCycle, currentIterationOfDFS);

                /* Increment the counter for the number of deadlocks when a deadlock is found for Task 4.
                 * Additionally, whenever a deadlock/cycle is found, the program will go through the cycle
                 * again to find the smallest process ID in the cycle to terminate and add it to a dynamic array
                 * for sorting later. */
                if(isDeadlocked == DEADLOCK_DETECTED && nodeInCycle != NULL) {
                    findSmallestProcessIDToTerminate(nodeInCycle, &processToTerminateID);
                    (*deadlockedProcessIDs)[*numDeadlocks] = processToTerminateID;
                    (*numDeadlocks)++;
                }

                /* At this point we know that there was a call to the visitNode (DFS) function and as such
                 * the counter tracking the current iteration of the DFS is incremented. */
                currentIterationOfDFS++;
            }
            /* Set everything back to default values as if there was no deadlock for the next iteration of the
             * DFS call which starts from another unvisited process node in the hash table. */
            isDeadlocked = false;
            nodeInCycle = NULL;
            curLLNode = curLLNode->next;
        }
    }
}

/* Standard insertion sort algorithm implementation to sort the process IDs. The findSmallestProcessIDToTerminate
 * already finds the smallest ID among the potential processes to terminate in a given cycle; this function
 * additionally sorts those process IDs in ascending order to meet the requirements of the project spec. */
void sortProcessIDs(int **deadlockedProcessIDs, int numDeadlocks) {
    int j, temp;

    for(int i = 1; i < numDeadlocks; i++) {
        int curProcessID = (*deadlockedProcessIDs)[i];
        j = i - 1;

        while(j >= 0 && (*deadlockedProcessIDs)[j] > curProcessID) {
            temp = (*deadlockedProcessIDs)[j+1];
            (*deadlockedProcessIDs)[j+1] = (*deadlockedProcessIDs)[j];
            (*deadlockedProcessIDs)[j] = temp;

            j--;
        }
    }
}