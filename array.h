#ifndef COMP30023_PROJECT_1_ARRAY_H
#define COMP30023_PROJECT_1_ARRAY_H

#include <stdlib.h>
#include <assert.h>

void sortProcessIDs(int **deadlockedProcessIDs, int numDeadlocks);

void reallocCheckArray(int **deadlockedProcessIDs, int *numDeadlocks, int *curMaxNumProcessIDs);

#endif //COMP30023_PROJECT_1_ARRAY_H
