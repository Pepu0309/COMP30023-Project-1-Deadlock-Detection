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

void reallocCheckArray(uint32_t **deadlockedProcessIDs, uint32_t *numDeadlocks, uint32_t *curMaxNumProcessIDs) {
    if(*numDeadlocks >= *curMaxNumProcessIDs) {
        *curMaxNumProcessIDs *= 2;
        *deadlockedProcessIDs = (uint32_t *) realloc (*deadlockedProcessIDs, *curMaxNumProcessIDs*sizeof(uint32_t));
        assert(*deadlockedProcessIDs != NULL);
    }
}