#ifndef COMP30023_PROJECT_1_ARRAY_H
#define COMP30023_PROJECT_1_ARRAY_H

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

void sortProcessIDs(uint32_t **deadlockedProcessIDs, uint32_t numDeadlocks);

void reallocCheckArray(uint32_t **deadlockedProcessIDs, uint32_t *numDeadlocks, uint32_t *curMaxNumProcessIDs);

#endif //COMP30023_PROJECT_1_ARRAY_H
