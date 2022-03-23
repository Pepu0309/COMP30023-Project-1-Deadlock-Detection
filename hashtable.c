#include "hashtable.h"

/* Takes the parameters newNodeID and newNodeType to create a new Resource Allocation Graph node (RAGNode) as well
 * as a linked list node that points to it. The linked list node is then inserted in the hash table. */
RAGNode_t *createNode(hashTableBucket_t hashTable[], uint32_t newNodeID, uint32_t newNodeType) {

    /* Allocate space for the new node and assign the ID and Type. Requests is set to 0 and visited set to false
     * (calloc should do the same but just in case). */
    RAGNode_t *thisNode = (RAGNode_t *) calloc (1, sizeof(RAGNode_t));
    thisNode->nodeID = newNodeID;
    thisNode->nodeType = newNodeType;
    thisNode->numRequests = 0;
    thisNode->visited = false;

    /* Linked list node is created and all it's pointers are initialised to null through calloc. The linked list
     * node is then assigned to point to the newly created RAG node. */
    linkedListNode_t *newLLNode = (linkedListNode_t *) calloc (1, sizeof(linkedListNode_t));
    newLLNode->RAGNode = thisNode;

    /* Node creation is done, the new linked list node, pointing to the new corresponding RAG node is inserted
     * into the hash table. */
    hashTableInsert(&(hashTable[newNodeID%NUM_BUCKETS]), newLLNode);

    return thisNode;
}

/* Takes a pointer of a hash table bucket which is implemented as a linked list and inserts a linked list node into
 * the hash table bucket. */
void hashTableInsert(hashTableBucket_t *bucketToInsert, linkedListNode_t *nodeToInsert) {
    /* If the head pointer is null, this is the first insert into the hash table bucket. */
    if(bucketToInsert->head == NULL) {
        bucketToInsert->head = nodeToInsert;
        bucketToInsert->tail = nodeToInsert;
    /* If not, then there is an existing node in this hash table bucket so go to the tail of this bucket
     * and then add the new node by setting the next of the current tail to the new node to insert. Then the
     * new node to insert will become the new tail of the bucket. */
    } else {
        bucketToInsert->tail->next = nodeToInsert;
        bucketToInsert->tail = nodeToInsert;
    }

}

/* Uses the hashing lookup property of a hash table to determine the linked list bucket to search for a duplicate
 * node and uses the parameters nodeID and nodeType to find said duplicate node and returns it if found. If no
 * duplicate node is found, NULL is return. */
RAGNode_t *searchDuplicate(hashTableBucket_t hashTable[], uint32_t nodeID, uint32_t nodeType) {

    /* Find the linked list to start the search for the duplicate node at and start at the head of the linked list. */
    hashTableBucket_t *bucketToSearch = (&hashTable[nodeID%NUM_BUCKETS]);
    linkedListNode_t *curLLNode = bucketToSearch->head;

    /* If the current linked list node isn't NULL, then we check to see if it's corresponding RAGNode has the same
     * parameters as the one passed to the function. If it does, then we have found a duplicate node. If we hit
     * the end in our search, that is when we encounter a null pointer when traversing the linked list, then
     * there is no duplicate node and the function returns null. */
    while (curLLNode != NULL){
        if(curLLNode->RAGNode->nodeID == nodeID && curLLNode->RAGNode->nodeType == nodeType) {
            return curLLNode->RAGNode;
        }

        curLLNode = curLLNode->next;
    }

    return NULL;
}

/* Goes through hash table and frees all the RAGNodes and Linked List Nodes */
void freeHashTable(hashTableBucket_t hashTable[]) {
    linkedListNode_t *curLLNode, *ptrToNextNode;
    for(int i = 0; i < NUM_BUCKETS; i++) {
        curLLNode = hashTable[i].head;
        while(curLLNode != NULL) {
            free(curLLNode->RAGNode);
            ptrToNextNode = curLLNode->next;
            free(curLLNode);
            curLLNode = ptrToNextNode;
        }
    }
}