#ifndef COMP30023_PROJECT_1_HASHTABLE_H
#define COMP30023_PROJECT_1_HASHTABLE_H

#define NUM_BUCKETS 71

#include "node.h"

RAGNode_t *createNode(hashTableBucket_t hashTable[], unsigned int newNodeID, unsigned int newNodeType,
                      int *numProcess, int *numFiles);

void hashTableInsert(hashTableBucket_t *bucketToInsert, linkedListNode_t *nodeToInsert);

RAGNode_t *searchDuplicate(hashTableBucket_t hashTable[], unsigned int nodeID, unsigned int nodeType);

void freeHashTable(hashTableBucket_t hashTable[]);

#endif //COMP30023_PROJECT_1_HASHTABLE_H
