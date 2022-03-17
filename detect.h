#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BOOL_TRUE 1
#define BOOL_FALSE 0

#define FILENAME_FLAG "-f"
#define EXECUTION_TIME_FLAG "-e"
#define PROCESS_ALLOCATION_FLAG "-c"

#include "node.h"

void parseResourceFile(char *filename, node_t **nodeArray, int *numProcess,  int *numFiles,
                       int *curMaxNumNodes);