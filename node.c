#include "node.h"

/* Recursive Depth First Search (DFS) function which visits a RAG node. Whenever a node is visited, the function
 * sets the visited attribute of the node to true and also sets the number of the iteration of DFS call this
 * node was visited on. This is so the function can find cycles as cycle are determined to be nodes which are
 * visited on the same iteration of DFS call. This is an alternative to the traditional stack approach. */
void visitNode(RAGNode_t *nodeToVisit, int *isDeadlocked, RAGNode_t **nodeInCycle, int currentIterationOfDFS) {

    /* If this node was visited before but it's not in this current iteration of DFS, then we immediately ignore
     * this node and exit. */
    if(nodeToVisit->visited == BOOL_TRUE) {
        /* If the node was visited in the same iteration of DFS call, then a cycle has been found and the node that
         * is part of the cycle is passed back to the detectDeadlocks program through a pointer. */
        if(nodeToVisit->numIterationOfDFSCall == currentIterationOfDFS) {
            *isDeadlocked = DEADLOCK_DETECTED;
            *nodeInCycle = nodeToVisit;
        }
        return;
    }

    /* As DFS is performed, nodes are marked as visited and marked with an unsigned int representing which
     * iteration of a DFS call the node is visited. */
    nodeToVisit->visited = BOOL_TRUE;
    nodeToVisit->numIterationOfDFSCall = currentIterationOfDFS;

    /* There can only potentially be a deadlock if the file a process is waiting for, is locked by another process.
     * It does not matter if a process node is requesting a file node when it is not locked by another file.
     * Hence, we skip 2 across from the current process as the next node of a process node is always a file node.
     * We are only concerned with checking process nodes for cycles/deadlocks. */
    if(nodeToVisit->dependencyTo->dependencyTo != NULL) {
        visitNode(nodeToVisit->dependencyTo->dependencyTo, isDeadlocked, nodeInCycle, currentIterationOfDFS);
    }

}
