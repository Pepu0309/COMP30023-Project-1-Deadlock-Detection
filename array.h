#ifndef COMP30023_PROJECT_1_ARRAY_H
#define COMP30023_PROJECT_1_ARRAY_H

#define DYNAMIC_ARRAY_INITIAL_STORAGE 5
#define DOUBLE_CUR_SPACE 2

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

void sortProcessIDs(uint32_t **deadlockedProcessIDs, uint32_t numDeadlocks);

void reallocCheckUnsignedIntArray(uint32_t **arrayToCheck, uint32_t numElements, uint32_t *curMaxNumElements);

#endif //COMP30023_PROJECT_1_ARRAY_H
