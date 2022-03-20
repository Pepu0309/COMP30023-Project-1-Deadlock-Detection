#ifndef COMP30023_PROJECT_1_NODE_H
#define COMP30023_PROJECT_1_NODE_H

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#define DOUBLE_CUR_SPACE 2

#define BOOL_TRUE 1
#define BOOL_FALSE 0

#define PROCESS_NODE 0
#define FILE_NODE 1

#define DEADLOCK_DETECTED -1

/* Struct which represents a Resource Allocation Graph node (RAGNode).
 * Contains:
 * 1. an int variable to represent the nodeType which is defined by constants PROCESS_NODE and FILE_NODE
 * 2. an unsigned int representing the ID of the node
 * 3. a pointer to another RAGNode which represents the dependency to another node (description in the struct body).
 * */
typedef struct RAGNode RAGNode_t;
struct RAGNode {
    int nodeType;
    unsigned int nodeID;

    /* A process node has a dependency to a file node as it is waiting for it.
     * A file node has a dependency to a process node as it is locked by it.
     * This creates a model similar to the Resource Allocation Graphs introduced in the lecture. */
    RAGNode_t *dependencyTo;
};

/* Struct which is a linked list node that points to it's corresponding RAGNode and has a pointer to the next
 * linked list node in the linked list. */
typedef struct linkedListNode linkedListNode_t;
struct linkedListNode {
    RAGNode_t *RAGNode;
    linkedListNode_t *next;
};

/* A linked list implementation of a hash table bucket which has a pointer to a node at the start of the linked list
 * and a pointer to a node at the end of the linked list, provided they exist. */
typedef struct hashTableBucket hashTableBucket_t;
struct hashTableBucket {
    linkedListNode_t *head;
    linkedListNode_t *tail;
};


#endif //COMP30023_PROJECT_1_NODE_H