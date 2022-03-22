#ifndef COMP30023_PROJECT_1_HASHTABLE_H
#define COMP30023_PROJECT_1_HASHTABLE_H

#define NUM_BUCKETS 71

#include "node.h"

RAGNode_t *createNode(hashTableBucket_t hashTable[], uint32_t newNodeID, uint32_t newNodeType);

void hashTableInsert(hashTableBucket_t *bucketToInsert, linkedListNode_t *nodeToInsert);

RAGNode_t *searchDuplicate(hashTableBucket_t hashTable[], uint32_t nodeID, uint32_t nodeType);

void freeHashTable(hashTableBucket_t hashTable[]);

#endif //COMP30023_PROJECT_1_HASHTABLE_H
