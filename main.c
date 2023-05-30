#include <stdio.h>
#include "MeasurementData.h"

void runExamples();

int main() {
    /* we will use a simple 4*4 matrix, predefined here: */
    const unsigned char buffer[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3, 4};

    /* create measurement data matrix */
    MD *pBuffer = MD_create(buffer, 4, 4);

    /* get statistics */
    unsigned int sum = MD_getSum(pBuffer, 1,1,3,3);
    double avg = MD_getAverage(pBuffer, 1,1,3,3);
    printf("Sum: %d Avg: %f\n", sum, avg);

    /* create deep copy of original buffer */
    MD *pCopy = MD_copy(pBuffer);

    /* delete both buffers */
    MD_delete(pBuffer);
    MD_delete(pCopy);

    runExamples();
    return 0;
}

/**
 * Function for running some examples and edge cases
 */
void runExamples() {
    printf("Running some examples of edge cases and more with this matrix:\n");

    /* first create demo matrix, we use the 'J' - matrix of ones for easy demonstrating purpose */
    const unsigned char buffer[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // this one is better for debugging
    MD_printMatrix(buffer,4,4);

    /* create the measurement data buffer we´ll use for demonstrating/testing the functionality */
    MD *pBuffer = MD_create(buffer, 4, 4);


    /* first case: get sum and avg of all elements */
    /* Are looks like this:
     * X X X X
     * X X X X
     * X X X X
     * X X X X
     */
    unsigned int sum = MD_getSum(pBuffer,0,0,3,3);
    double avg = MD_getAverage(pBuffer,0,0,3,3);
    printf("Sum and average of complete J matrix should be 16 and 1:\n");
    printf("Sum: %d Avg: %f\n", sum, avg);


    /* second case: get sum and avg of one element */
    /* Are looks like this:
     * X 1 1 1
     * 1 1 1 1
     * 1 1 1 1
     * 1 1 1 1
     */
    sum = MD_getSum(pBuffer,0,0,0,0);
    avg = MD_getAverage(pBuffer,0,0,0,0);
    printf("Sum and average of (0,0) (0,0) submatrix should be both 1:\n");
    printf("Sum: %d Avg: %f\n", sum, avg);


    /* third case: get sum and avg of overlapping submatrix */
    /* Are looks like this:
     * 1 1 1 1
     * 1 1 1 1
     * 1 1 X X X
     * 1 1 X X X
     *     X X X
     */
    sum = MD_getSum(pBuffer,2,2,4,4);
    avg = MD_getAverage(pBuffer,2,2,4,4);
    printf("Sum and average of (2,2) (4,4) submatrix should be 4 and 0.4444 \n");
    printf("Sum: %d Avg: %f\n", sum, avg);


    /* fourth case: get sum and avg of 'inverted rectangle' */
    /* Are looks like this:
     * 1 1 1 1
     * 1 X X X ->  p2 - +
     * 1 X X X ->  + - p1
     * 1 1 1 1
     */
    sum = MD_getSum(pBuffer,2,3,1,1);
    avg = MD_getAverage(pBuffer,3,2,1,1);
    printf("Sum and average of (2,3) (1,1) inverted rectangle should be 6 and 1 \n");
    printf("Sum: %d Avg: %f\n", sum, avg);


    /* fifth case: get sum and avg of 'inverted rectangle' */
    /* Are looks like this:
     * 1 1 1 1
     * 1 X X X ->  + - p1
     * 1 X X X ->  p2 - +
     * 1 1 1 1
     */
    sum = MD_getSum(pBuffer,3,1,1,2);
    avg = MD_getAverage(pBuffer,3,1,1,2);
    printf("Sum and average of (3,1) (1,2) inverted rectangle should be 6 and 1 \n");
    printf("Sum: %d Avg: %f\n", sum, avg);


    /* sixth case: get sum and avg of matrix which is out of bounds */
    /* Are looks like this:
     * 1 1 1 1
     * 1 1 1 1
     * 1 1 1 1 X X
     * 1 1 1 1 X X
     */
    sum = MD_getSum(pBuffer,4,3,4,4);
    avg = MD_getAverage(pBuffer,4,3,4,4);
    printf("Sum and average of (4,3) (4,4) 'out of bounds' should be both 0 \n");
    printf("Sum: %d Avg: %f\n", sum, avg);

    /* seventh case: get sum and avg of matrix which is also out of bounds */
    /* Are looks like this:
     * 1 1 1 1
     * 1 1 1 1
     * 1 1 1 1
     * 1 1 1 1
     *     X X
     */
    sum = MD_getSum(pBuffer,4,2,4,3);
    avg = MD_getAverage(pBuffer,4,2,4,3);
    printf("Sum and average of (4,2) (4,3) 'out of bounds' should be both 0 \n");
    printf("Sum: %d Avg: %f\n", sum, avg);
}