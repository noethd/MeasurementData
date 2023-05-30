//
// Created by Dominik on 30.05.23.
//

#ifndef MD_H
#define MD_H
#define MAX_MATRIX_SIZE_X_Y 4096
#include <stdlib.h>
#include "stdio.h"
#include <string.h>
#include <stdbool.h>

typedef struct MD
{
    unsigned char* pMatrix;
    unsigned char* pLookupMatrix;
    int width;
    int height;
} MD;

MD*  MD_create (const unsigned char* s, int width, int height);
void MD_delete (MD*);
MD*  MD_copy (const MD*);

unsigned int MD_getSum (const MD*, int x0, int y0, int x1, int y1);
double       MD_getAverage (const MD*, int x0, int y0, int x1, int y1);

/* helper functions */
void MD_precalculateMatrix(MD *pBuffer);
void MD_printMatrix(unsigned char* pMatrix, int w, int h);
bool isOutOfBounds(const MD* pBuffer, int x0, int y0, int x1, int y1);
bool correctCoordinateOrder(const MD* pBuffer, int *x0, int *y0, int *x1, int *y1);
void swap(int *a, int *b);

#endif //MD_H
