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
        return NULL;
    }
    if(height < 0 || height > MAX_MATRIX_SIZE_X_Y) {
        return NULL;
    }

    MD *pMD = malloc(sizeof(MD));
    if (pMD == NULL){
        return NULL;
    }

    pMD->height = height;
    pMD->width = width;

    size_t sizeOfMatrix = sizeof(unsigned char) * width * height;

    /* simple and straight forward null checks, could be done better by putting in own function */
    pMD->pMatrix = malloc(sizeOfMatrix);
    if(pMD->pMatrix == NULL) {
        free(pMD);
        return NULL;
    }
    pMD->pLookupMatrix = malloc(sizeOfMatrix);
    if(pMD->pLookupMatrix  == NULL) {
        free(pMD->pMatrix);
        free(pMD);
        return NULL;
    }

    memcpy(pMD->pMatrix, pData, sizeOfMatrix);
    memcpy(pMD->pLookupMatrix, pMD->pMatrix, sizeOfMatrix);

    MD_precalculateMatrix(pMD);
    return pMD;
}

/**
 * Function to safe delete a MeasurementData object
 * @param MD *pData the object to delete
 */
void MD_delete (MD *pData) {
    if(!pData)
        return;

    if(pData->pMatrix)
        free(pData->pMatrix);

    if(pData->pLookupMatrix)
        free(pData->pLookupMatrix);

    free(pData);
}
/**
 * Function to deep copy a MeasurementData object
 * @param MD *pData the original MeasurementData object
 */
MD* MD_copy (const MD *pOriginalData) {
    MD *pCopy = malloc(sizeof(MD));
    if(pCopy == NULL) {
        return NULL;
    }
    /* can be done by memcpy too */
    pCopy->width = pOriginalData->width;
    pCopy->height = pOriginalData->height;

    size_t sizeOfMatrix = sizeof(unsigned char) * pOriginalData->width * pOriginalData->height;

    /* simple and straight forward null checks, could be done better by putting in own function */
    pCopy->pMatrix = malloc(sizeOfMatrix);
    if(pCopy->pMatrix == NULL) {
        free(pCopy);
        return NULL;
    }
    pCopy->pLookupMatrix = malloc(sizeOfMatrix);
    if(pCopy->pLookupMatrix  == NULL) {
        free(pCopy->pMatrix);
        free(pCopy);
        return NULL;
    }
    memcpy(pCopy->pMatrix, pOriginalData->pMatrix, sizeOfMatrix);
    memcpy(pCopy->pLookupMatrix, pOriginalData->pLookupMatrix, sizeOfMatrix);

    return pCopy;
}

/**
 * Function precalculate the lookup matrix for enabling o(1) calulations of sum and avg
 * @param MD *pData the original MeasurementData object
 */
void MD_precalculateMatrix(MD *pBuffer) {
    /* the first part of the magic is done here */

    /* preprocess the first row, calculate the values from original matrix to precalculatedMatrix like this:
     * i = i+(i-1) e.g [0,1,2,3] -> [0,1,3,7]
     */
    for (int i = 1; i < pBuffer->width; i++) {
        pBuffer->pLookupMatrix[i] = pBuffer->pMatrix[i] + pBuffer->pLookupMatrix[i - 1];
    }

    /* do the same thing with the first column */
    for (int i = 1; i < pBuffer->height; i++) {
        pBuffer->pLookupMatrix[i * pBuffer->width] = pBuffer->pMatrix[i * pBuffer->width] + pBuffer->pLookupMatrix[(i - 1) * pBuffer->width];
    }

    /* preprocess the rest of the matrix like before */
    for (int i = 1; i < pBuffer->height; i++)
    {
        for (int j = 1; j < pBuffer->width; j++)
        {
            pBuffer->pLookupMatrix[i + j * pBuffer->width] = pBuffer->pMatrix[i + j * pBuffer->width]
                                                             + pBuffer->pLookupMatrix[(i - 1) + j * pBuffer->width]
                                                             + pBuffer->pLookupMatrix[i + (j - 1) * pBuffer->width]
                                                             - pBuffer->pLookupMatrix[(i - 1) + (j - 1) * pBuffer->width];
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
    /* edge case, everything is out of bounds */
    if(isOutOfBounds(pBuffer, x0, y0, x1, y1))
        return 0;

    /* prepare parameter for correct order and boundary´s*/
    correctCoordinateOrder(pBuffer, &x0, &y0, &x1, &y1);
    clampCoordinates(pBuffer, &x0, &y0, &x1, &y1);

    /* the second part of the magic is done here.
     * Calculation is done, by subtracting sub-matrices from each other
     * */
    int total = pBuffer->pLookupMatrix[x1 + y1 * pBuffer->width];

    if (y0 - 1 >= 0) {
        total -= pBuffer->pLookupMatrix[x1 + (y0 - 1) * pBuffer->width];
    }

    if (x0 - 1 >= 0) {
        total -= pBuffer->pLookupMatrix[(x0 - 1) + y1 * pBuffer->width];
    }

    if (x0 - 1 >= 0 && y0 - 1 >= 0) {
        total += pBuffer->pLookupMatrix[(x0 - 1) + (y0 - 1) * pBuffer->width];
    }

    return total;
}
/**
 * Function correct the order of the given coordinates
 * if p2 is nearer at 0,0 than p1, swap them so p1 is always upper left of the rectangle
 * @param MD *pBuffer the buffer to get the values from
 * @param int x0 x coordinate of first point of the submatrix
 * @param int y0 y coordinate of first point of the submatrix
 * @param int x1 x coordinate of second point of the submatrix
 * @param int x2 y coordinate of second point of the submatrix
 */
void correctCoordinateOrder(const MD *pBuffer, int *x0, int *y0, int *x1, int *y1) {
    /* check if points are not in the right order, if needed swap them */
    if (*x1 < *x0) {
        swap(x1,x0);
    }
    if (*y1 < *y0) {
        swap(y1,y0);
    }
}

/**
 * Function for clamping the coordinates to max size of given buffer
 * will handle data inplace
 * @param const MD *pBuffer the buffer to get the values from
 * @param int x0 x coordinate of first point of the submatrix
 * @param int y0 y coordinate of first point of the submatrix
 * @param int x1 x coordinate of second point of the submatrix
 * @param int x2 y coordinate of second point of the submatrix
 */
void clampCoordinates(const MD* pBuffer, int *x0, int *y0, int *x1, int *y1) {
    /* clamp coordinates to max buffer size */
    if(*x1 >= pBuffer->width)
        *x1 = pBuffer->width - 1;
    if(*y1 >= pBuffer->height)
        *y1 = pBuffer->height - 1;
}


/**
 * Function for checking if coordinates are out of the given matrix
 * @param const MD *pBuffer the buffer to get the values from
 * @param int x0 x coordinate of first point of the submatrix
 * @param int y0 y coordinate of first point of the submatrix
 * @param int x1 x coordinate of second point of the submatrix
 * @param int x2 y coordinate of second point of the submatrix
 * @return true if both coordinates are outside the buffer
 */
bool isOutOfBounds(const MD *pBuffer, int x0, int y0, int x1, int y1) {
    /* check left boundary's */
    if(x0 >= pBuffer->width && x1 >= pBuffer->width)
        return true;
    /* check lower boundary's */
    if(y0 >= pBuffer->height && y1 >= pBuffer->height)
        return true;

    return false;
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
    if (sum == 0) return 0;

    correctCoordinateOrder(pBuffer, &x0, &y0, &x1, &y1);
    double elements = (y1 + 1 - y0) * (x1 + 1 -x0);
    return sum / elements;
}

/**
 * Function for fast swapping inplace, search for 'inplace swap' or 'XOR swap'
 * XOR Swap would also be possible
 * @param a
 * @param b
 */
void swap(int *a, int *b) {
    if (a != b)
    {
        *a = *a + *b;
        *b = *a - *b;
        *a = *a - *b;
    }
}
#endif //MD_C
