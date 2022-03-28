#include "array.h"

/* Implementation of the comparator to use the C library function qsort() to sort an array of uint32_t. This function
 * was created by referencing code from the website
 * https://stackoverflow.com/questions/36681906/c-qsort-doesnt-seem-to-work-with-unsigned-long
 * and adapted for use. */
int QSortComparator(const void *a, const void *b) {
    /* As the arguments a and b are of type const void * (according to the C requirements for the comparator function
     * to use qsort), type cast both a and b to pointers of type (const uint32_t *). Then dereference the pointer
     * after the type cast and store it as a variable of type const_uint32_t x and y to use easily in the
     * next few lines of comparison instead of having to type cast the variables each time when referring to them. */
    const uint32_t x = *(const uint32_t *)a;
    const uint32_t y = *(const uint32_t *)b;

    /* Return negative number (-1) when x is less than y, positive number (1) when x is more than y, and otherwise
     * they must be equal and so return 0. This comparator is then used as part of C library qsort function to
     * sort the deadlocked process IDs. */
    if(x < y) {
        return -1;
    }

    if (x > y) {
        return 1;
    }

    return 0;
}

/* General function that checks whether there is a need to realloc an unsigned int array. */
void reallocCheckUnsignedIntArray(uint32_t **arrayToCheck, uint32_t numElements, uint32_t *curMaxNumElements) {
    if(numElements >= *curMaxNumElements) {
        *curMaxNumElements *= DOUBLE_CUR_SPACE;
        *arrayToCheck = (uint32_t *) realloc (*arrayToCheck, (*curMaxNumElements) * sizeof(uint32_t));
        assert(arrayToCheck != NULL);
    }
}

