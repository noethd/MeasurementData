//
// Created by Dominik on 30.05.23.
//

#ifndef MD_C
#define MD_C

#include "MeasurementData.h"

/**
 * Function to create a new MeasurementData object
 * @param const unsigned char *pData  array with buffer data
 * @param int width  cols of matrix
 * @param height  rows of matrix
 * @return a pointer to new created MD struct
 */
MD* MD_create (const unsigned char *pData, int width, int height) {

    /* if size is out of bounds always set to zero - its also possible to assert or return nullptr. */
    if(width < 0 || width > MAX_MATRIX_SIZE_X_Y) {
        width = 0;
    }
    if(height < 0 || height > MAX_MATRIX_SIZE_X_Y) {
        height = 0;
    }

    MD *pMD = malloc(sizeof(MD));
    pMD->height = height;
    pMD->width = width;

    size_t sizeOfMatrix = sizeof(unsigned char) * width * height;
    pMD->pMatrix = malloc(sizeOfMatrix);
    memcpy(pMD->pMatrix, pData, sizeOfMatrix);
    pMD->pPrecalculatedMatrix = malloc(sizeOfMatrix);
    memcpy(pMD->pPrecalculatedMatrix, pMD->pMatrix, sizeOfMatrix);
    MD_precalculateMatrix(pMD);
    return pMD;
}

/**
 * Function to safe delete a MeasurementData object
 * @param MD *pData the object to delete
 */
void MD_delete (MD *pData) {
    if(pData->pMatrix)
        free(pData->pMatrix);

    if(pData->pPrecalculatedMatrix)
        free(pData->pPrecalculatedMatrix);

    if(pData)
        free(pData);
}
/**
 * Function to deep copy a MeasurementData object
 * @param MD *pData the original MeasurementData object
 */
MD* MD_copy (const MD *pOriginalData) {
    MD *pCopy = malloc(sizeof(MD));

    // can be done by memcpy too
    pCopy->width = pOriginalData->width;
    pCopy->height = pOriginalData->height;

    size_t sizeOfMatrix = sizeof(unsigned char) * pOriginalData->width * pOriginalData->height;
    pCopy->pMatrix = malloc(sizeOfMatrix);
    pCopy->pPrecalculatedMatrix = malloc(sizeOfMatrix);

    memcpy(pCopy->pMatrix, pOriginalData->pMatrix, sizeOfMatrix);
    memcpy(pCopy->pPrecalculatedMatrix, pOriginalData->pPrecalculatedMatrix, sizeOfMatrix);

    return pCopy;
}

/**
 * Function precalculate the lookup matrix for enabling o(1) calulations of sum and avg
 * @param MD *pData the original MeasurementData object
 */
void MD_precalculateMatrix(MD *pBuffer) {

    // preprocess the first row
    for (int i = 1; i < pBuffer->width; i++) {
        pBuffer->pPrecalculatedMatrix[i] = pBuffer->pMatrix[i] + pBuffer->pPrecalculatedMatrix[i - 1];
    }

    // preprocess the first column
    for (int i = 1; i < pBuffer->height; i++) {
        pBuffer->pPrecalculatedMatrix[i * pBuffer->width] = pBuffer->pMatrix[i * pBuffer->width] + pBuffer->pPrecalculatedMatrix[(i - 1) * pBuffer->width];
    }

    // preprocess the rest of the matrix
    for (int i = 1; i < pBuffer->height; i++)
    {
        for (int j = 1; j < pBuffer->width; j++)
        {
            pBuffer->pPrecalculatedMatrix[i + j * pBuffer->width] = pBuffer->pMatrix[i + j * pBuffer->width]
                    + pBuffer->pPrecalculatedMatrix[(i - 1) + j * pBuffer->width]
                    + pBuffer->pPrecalculatedMatrix[i + (j - 1) * pBuffer->width]
                    - pBuffer->pPrecalculatedMatrix[(i - 1) + (j - 1) * pBuffer->width];
        }
    }
}

/**
 * Helper function for printing any matrix to console
 * @param unsigned char* pMatrix the matrix to print
 * @param int rows number of rows
 * @param int cols number of colums
 */
void MD_printMatrix(unsigned char* pMatrix, int rows, int cols) {
    for(int j = 0; j < rows; j++) {
        for(int i = 0; i < cols; i++) {
            printf("%d ", pMatrix[i + j * rows]);
        }
        printf("\n");
    }
}

/**
 * Function for calculating the sum of the submatrix
 * Runs in constant time, due to the preprocessed matrix.
 * @param const MD *pBuffer the buffer to get the values from
 * @param int x0 x coordinate of first point of the submatrix
 * @param int y0 y coordinate of first point of the submatrix
 * @param int x1 x coordinate of second point of the submatrix
 * @param int x2 y coordinate of second point of the submatrix
 */
unsigned int MD_getSum (const MD *pBuffer, int x0, int y0, int x1, int y1) {
    if(x1 >= pBuffer->width)
        x1 = pBuffer->width - 1;
    if(y1 >= pBuffer->height)
        y1 = pBuffer->height - 1;

    int total = pBuffer->pPrecalculatedMatrix[x1 + y1 * pBuffer->width];

    if (y0 - 1 >= 0) {
        total -= pBuffer->pPrecalculatedMatrix[x1 + (y0 - 1) * pBuffer->width];
    }

    if (x0 - 1 >= 0) {
        total -= pBuffer->pPrecalculatedMatrix[(x0 - 1) + y1 * pBuffer->width];
    }

    if (x0 - 1 >= 0 && y0 - 1 >= 0) {
        total += pBuffer->pPrecalculatedMatrix[(x0 - 1) + (y0 - 1) * pBuffer->width];
    }

    return total;
}

/**
 * Function for calculating average of the submatrix
 * Runs in constant time, due to the preprocessed matrix.
 * @param const MD *pBuffer the buffer to get the values from
 * @param int x0 x coordinate of first point of the submatrix
 * @param int y0 y coordinate of first point of the submatrix
 * @param int x1 x coordinate of second point of the submatrix
 * @param int x2 y coordinate of second point of the submatrix
 */
double MD_getAverage (const MD *pBuffer, int x0, int y0, int x1, int y1) {
    int sum = MD_getSum(pBuffer,x0,y0,x1,y1);
    double elements = (y1 + 1 - y0) * (x1 + 1 -x0);
    return sum / elements;
}
#endif //MD_C
