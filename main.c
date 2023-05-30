#include <stdio.h>
#include "MeasurementData.h"

int main() {
    printf("Original Mat:\n");

    //const unsigned char buffer[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3, 4};
    const unsigned char buffer[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // this one is better for debugging
    MD_printMatrix(buffer, 4, 4);

    /* create measurement data matrix */
    MD *pBuffer = MD_create(buffer, 4, 4);

    /* get statistics */
    unsigned int sum = MD_getSum(pBuffer, 2,0,3,3);
    double avg = MD_getAverage(pBuffer, 0,0,4,4);

    printf("Sum: %d Avg: %f", sum, avg);

    /* create deep of original buffer */
    MD *pCopy = MD_copy(pBuffer);

    /* delete both buffers */
    MD_delete(pBuffer);
    MD_delete(pCopy);

    return 0;
}
