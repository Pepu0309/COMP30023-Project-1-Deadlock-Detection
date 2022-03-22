#include "node.h"

/* Recursive Depth First Search (DFS) function which visits a RAG node. Whenever a node is visited, the function
 * sets the visited attribute of the node to true and also sets the number of the iteration of DFS call this
 * node was visited on. This is so the function can find cycles as cycle are determined to be nodes which are
 * visited on the same iteration of DFS call. This is an alternative to the traditional stack approach. */
void visitNode(RAGNode_t *nodeToVisit, bool *isDeadlocked, RAGNode_t **nodeInCycle, uint32_t currentIterationOfDFS) {

    /* If this node was visited before but it's not in this current iteration of DFS, then we immediately ignore
     * this node and exit. */
    if(nodeToVisit->visited == true) {
        /* If the node was visited in the same iteration of DFS call, then a cycle has been found and the node that
         * is part of the cycle is passed back to the detectDeadlocks program through a pointer. */
        if(nodeToVisit->numIterationOfDFSCall == currentIterationOfDFS) {
            *isDeadlocked = true;
            *nodeInCycle = nodeToVisit;
        }
        return;
    }

    /* At this point, the node has not been visited in a previous iteration of DFS. As DFS is performed, the node
     * is then marked as visited and marked with an unsigned int representing which iteration of a DFS call
     * the node is visited. */
    nodeToVisit->visited = true;
    nodeToVisit->numIterationOfDFSCall = currentIterationOfDFS;

    /* There can only potentially be a deadlock if the file a process is waiting for, is locked by another process.
     * It does not matter if a process node is requesting a file node when it is not locked by another file.
     * Hence, we skip 2 across from the current process as the next node of a process node is always a file node.
     * We are only concerned with checking process nodes for cycles/deadlocks. */
    if(nodeToVisit->dependencyTo->dependencyTo != NULL) {
        visitNode(nodeToVisit->dependencyTo->dependencyTo, isDeadlocked, nodeInCycle, currentIterationOfDFS);
    }

}

/* Takes a RAG node which the visitNode function has determined to be in a cycle, go through the every process
 * node of the cycle and find the smallest process ID. This will then be deemed to be the process to terminate
 * in order to break the deadlock. */
void findSmallestProcessIDToTerminate(RAGNode_t *startNodeInCycle, uint32_t *processToTerminateID) {

    RAGNode_t *curNode = startNodeInCycle;

    /* By default, the ID of the process to terminate is the starting node in the cycle. This starting node
     * is also marked by the program to be in a cycle. */
    *processToTerminateID = startNodeInCycle->nodeID;

    /* Traverses the cycle and finds the smallest ID of the potential processes to terminate to solve the
     * given deadlock. The program traverses the cycle by going to the next process node until it reaches the
     * starting node. It also marks each node to be in a cycle. */
    do {
        if(curNode->dependencyTo->dependencyTo != NULL) {
            curNode = curNode->dependencyTo->dependencyTo;
        }
        if((curNode->nodeID) < *processToTerminateID) {
            *processToTerminateID = curNode->nodeID;
        }
    } while (curNode != startNodeInCycle);

}