#include "detect.h"

int main(int argc, char **argv) {

    char *filename;
    int filenameFlag = BOOL_FALSE;
    int executionTimeFlag = BOOL_FALSE;
    int processAllocationFlag = BOOL_FALSE;


    for(int i = 0; i < argc; i++){

        if(filenameFlag) {
            filename = argv[i];
            filenameFlag = BOOL_FALSE;
        }

        if(strcmp(argv[i], FILENAME_FLAG) == 0) {
            filenameFlag = BOOL_TRUE;
        } else if (strcmp(argv[i], EXECUTION_TIME_FLAG) == 0){
            executionTimeFlag = BOOL_TRUE;
        } else if (strcmp(argv[i], PROCESS_ALLOCATION_FLAG) == 0){
            processAllocationFlag = BOOL_TRUE;
        }
    }
    /*
    printf("%s\n", filename);
    printf("%d\n", executionTimeFlag);
    printf("%d\n", processAllocationFlag);
    */

    /* The current node number can be derived by adding numProcess and numFiles. curMaxNumNodes variable is used
     * to help determine if there is a need to call realloc before creating a new node. */
    int numProcess = 0;
    int numFiles = 0;
    int curMaxNumNodes = INITIAL_NODES;

    node_t *nodeArray = (node_t *) malloc (sizeof(node_t) * INITIAL_NODES);
    assert(nodeArray != NULL);

    parseResourceFile(filename, &nodeArray, &numProcess, &numFiles, &curMaxNumNodes);


    printf("Processes %d\n", numProcess);
    printf("Files %d\n", numFiles);
    return 0;
}

void parseResourceFile(char *filename, node_t **nodeArray, int *numProcess,  int *numFiles,
                       int *curMaxNumNodes) {

    FILE *fp = fopen(filename, "r");

    unsigned int processID, lockedFileID, requiredFileID;

    node_t *processNode, *lockedFileNode, *requiredFileNode;

    while(fscanf(fp, "%u %u %u", &processID, &lockedFileID, &requiredFileID) == 3) {

        processNode = createNode(nodeArray, processID, PROCESS_NODE,
                                         numProcess, numFiles, curMaxNumNodes);
        (*numProcess)++;

        if((lockedFileNode = searchDuplicate(nodeArray, lockedFileID, FILE_NODE, *numProcess, *numFiles)) == NULL) {
            lockedFileNode = createNode(nodeArray, lockedFileID, FILE_NODE,
                               numProcess, numFiles, curMaxNumNodes);
            (*numFiles)++;
        }

        if((requiredFileNode = searchDuplicate(nodeArray, requiredFileID, FILE_NODE, *numProcess, *numFiles)) == NULL) {
            requiredFileNode = createNode(nodeArray, requiredFileID, FILE_NODE,
                               numProcess, numFiles, curMaxNumNodes);
            (*numFiles)++;
        }

        /* Now we handle the dependencies of the resource nodes */
        processNode->dependencyTo = requiredFileNode;
        lockedFileNode->dependencyTo = processNode;

    }

}

