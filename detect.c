#include "detect.h"

int main(int argc, char **argv) {

    int filenameFlag = BOOL_FALSE;
    int executionTimeFlag = BOOL_FALSE;
    int processAllocationFlag = BOOL_FALSE;

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
            filenameFlag = BOOL_FALSE;
        }

        /* Checks which of the 3 flags possible the current argument in argv is. */
        if(strcmp(argv[i], FILENAME_FLAG) == 0) {
            filenameFlag = BOOL_TRUE;
        } else if (strcmp(argv[i], EXECUTION_TIME_FLAG) == 0){
            executionTimeFlag = BOOL_TRUE;
        } else if (strcmp(argv[i], PROCESS_ALLOCATION_FLAG) == 0){
            processAllocationFlag = BOOL_TRUE;
        }
    }

    /* Print statistics for task 1 */
    printf("Processes %d\n", numProcess);
    printf("Files %d\n", numFiles);

    /* If the execution time flag -e was given as an argument, calculate the minimum execution time
     * of the processes; otherwise, detect deadlocks in the processes. */
    if(executionTimeFlag) {
    } else {
        /* to be implemented. */
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

        processNode = createNode(hashTable, processID, PROCESS_NODE,
                                         numProcess, numFiles);
        (*numProcess)++;


        /* For both the file nodes (locked file and required file), we search the hashTable for a duplicate node. If
         * we do find one, then the function will return the node. If we don't find one, the function returns NULL
         * and the program then creates a new node based on the parameters. */
        if((lockedFileNode = searchDuplicate(hashTable, lockedFileID, FILE_NODE, *numProcess, *numFiles)) == NULL) {
            lockedFileNode = createNode(hashTable, lockedFileID, FILE_NODE,
                               numProcess, numFiles);
            (*numFiles)++;
        }

        if((requiredFileNode = searchDuplicate(hashTable, requiredFileID, FILE_NODE, *numProcess, *numFiles)) == NULL) {
            requiredFileNode = createNode(hashTable, requiredFileID, FILE_NODE,
                               numProcess, numFiles);
            (*numFiles)++;
        }

        /* Now we handle the dependencies of the resource nodes. A process node always has a dependency to the file
         * it requires. The file being required also stores the number of time it is requested in the system to
         * be used for calculating execution time. A locked file node always has a dependency to the process it
         * is locked by. Even if one of the nodes is a duplicate, this behaviour is intended to create the
         * resource allocation graph. */
        processNode->dependencyTo = requiredFileNode;
        (requiredFileNode->numRequests)++;
        lockedFileNode->dependencyTo = processNode;

    }

    fclose(fp);
}






