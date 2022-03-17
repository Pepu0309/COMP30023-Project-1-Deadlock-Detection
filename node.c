

#include "node.h"

node_t *createNode(node_t **nodeArray, unsigned int newNodeID, unsigned int newNodeType,
    int *numProcess, int *numFiles, int *curMaxNumNodes) {

    // Realloc if there's not enough space in the array for the new node.
    if((*numProcess + *numFiles) >= *curMaxNumNodes) {
        (*curMaxNumNodes) *= DOUBLE_CUR_SPACE;
        *nodeArray = (node_t *) realloc(*nodeArray, (*curMaxNumNodes) * sizeof(node_t));
        assert(*nodeArray != NULL);
    }

    (*nodeArray)[(*numProcess + *numFiles)].nodeID = newNodeID;
    (*nodeArray)[(*numProcess + *numFiles)].nodeType = newNodeType;

    return &((*nodeArray)[(*numProcess + *numFiles)]);
}

/* Iterate over all nodes and if we find one that shares the same type (process or file) and same ID, then this
 * node already exists and the function return BOOL_TRUE. Otherwise, BOOL_FALSE is returned.  */
node_t *searchDuplicate(node_t **nodeArray, unsigned int nodeID, unsigned int nodeType,
    int numProcess, int numFiles) {

    for(int i = 0; i < (numProcess + numFiles); i++) {
        if((*nodeArray)[i].nodeID == nodeID && (*nodeArray)[i].nodeType == nodeType) {
            return &((*nodeArray)[i]);
        }
    }

    return NULL;
}