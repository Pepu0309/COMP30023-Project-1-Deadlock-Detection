#include "array.h"

/* Standard insertion sort algorithm implementation to sort the process IDs. The findSmallestProcessIDToTerminate
 * already finds the smallest ID among the potential processes to terminate in a given cycle; this function
 * additionally sorts those process IDs in ascending order to meet the requirements of the project spec. */
void sortProcessIDs(uint32_t **deadlockedProcessIDs, uint32_t numDeadlocks) {
    uint32_t j, temp;

    for(uint32_t i = 1; i < numDeadlocks; i++) {
        uint32_t curProcessID = (*deadlockedProcessIDs)[i];
        j = i - 1;

        while(j >= 0 && (*deadlockedProcessIDs)[j] > curProcessID) {
            temp = (*deadlockedProcessIDs)[j+1];
            (*deadlockedProcessIDs)[j+1] = (*deadlockedProcessIDs)[j];
            (*deadlockedProcessIDs)[j] = temp;

            j--;
        }
    }
}

/* General function that checks whether there is a need to realloc any unsigned int array in the whole program. */
void reallocCheckUnsignedIntArray(uint32_t **arrayToCheck, uint32_t numElements, uint32_t *curMaxNumElements) {
    if(numElements >= *curMaxNumElements) {
        *curMaxNumElements *= 2;
        *arrayToCheck = (uint32_t *) realloc (*arrayToCheck, (*curMaxNumElements) * sizeof(uint32_t));
        assert(arrayToCheck != NULL);
    }
}

