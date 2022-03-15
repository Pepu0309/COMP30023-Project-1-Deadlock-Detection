#include <stdio.h>
#include <string.h>
#include "detect.h"

int main(int argc, char **argv) {

    char *filename;
    int filenameFlag = BOOL_FALSE;
    int executionTimeFlag = BOOL_FALSE;
    int processAllocationFlag = BOOL_FALSE;

    for(int i = 0; i < argc; i++){

        if(filenameFlag) {
            filename = argv[i];
            filenameFlag = BOOL_FALSE;
        }

        if(strcmp(argv[i], FILENAME_FLAG) == 0) {
            filenameFlag = BOOL_TRUE;
        } else if (strcmp(argv[i], EXECUTION_TIME_FLAG) == 0){
            executionTimeFlag = BOOL_TRUE;
        } else if (strcmp(argv[i], PROCESS_ALLOCATION_FLAG) == 0){
            processAllocationFlag = BOOL_TRUE;
        }
    }

    printf("%d\n", filenameFlag);
    printf("%s\n", filename);
    printf("%d\n", executionTimeFlag);
    printf("%d\n", processAllocationFlag);
    return 0;
}
