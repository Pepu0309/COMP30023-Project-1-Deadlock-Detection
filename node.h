#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#define INITIAL_NODES 10
#define DOUBLE_CUR_SPACE 2

#define PROCESS_NODE 0
#define FILE_NODE 1

typedef struct node node_t;
struct node {
    int nodeType;
    unsigned int nodeID;
    /* A process node has a dependency to a file node as it is waiting for it.
     * A file node has a dependency to a process node as it is locked by it.
     * This creates a model similar to the Resource Allocation Graphs introduced in the lecture. */
    node_t *dependencyTo;
};

node_t *createNode(node_t **nodeArray, unsigned int newNodeID, unsigned int newNodeType,
    int *numProcess, int *numFiles, int *curMaxNumNodes);

node_t *searchDuplicate(node_t **nodeArray, unsigned int newNodeId, unsigned int newNodeType,
    int numProcess, int numFiles);