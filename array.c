#include "array.h"

/* Standard insertion sort algorithm implementation to sort the process IDs. The findSmallestProcessIDToTerminate
 * already finds the smallest ID among the potential processes to terminate in a given cycle; this function
 * additionally sorts those process IDs in ascending order to meet the requirements of the project spec. */
void sortProcessIDs(int **deadlockedProcessIDs, int numDeadlocks) {
    int j, temp;

    for(int i = 1; i < numDeadlocks; i++) {
        int curProcessID = (*deadlockedProcessIDs)[i];
        j = i - 1;

        while(j >= 0 && (*deadlockedProcessIDs)[j] > curProcessID) {
            temp = (*deadlockedProcessIDs)[j+1];
            (*deadlockedProcessIDs)[j+1] = (*deadlockedProcessIDs)[j];
            (*deadlockedProcessIDs)[j] = temp;

            j--;
        }
    }
}

void reallocCheckArray(int **deadlockedProcessIDs, int *numDeadlocks, int *curMaxNumProcessIDs) {
    if(*numDeadlocks >= *curMaxNumProcessIDs) {
        *curMaxNumProcessIDs *= 2;
        *deadlockedProcessIDs = (int *) realloc (*deadlockedProcessIDs, *curMaxNumProcessIDs*sizeof(int));
        assert(*deadlockedProcessIDs != NULL);
    }
}