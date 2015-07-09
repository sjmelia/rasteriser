#ifndef MATRIX4_H
#define MATRIX4_H

#include "vector4.h"

typedef struct matrix4
{
    double a1,a2,a3,a4;
    double b1,b2,b3,b4;
    double c1,c2,c3,c4;
    double d1,d2,d3,d4;
} matrix4;

matrix4* matrix4_create();
void matrix4_free(matrix4* matrix);
void matrix4_rotation(matrix4* matrix, double angle, double x, double y, double z);
void matrix4_translate(matrix4* matrix, double x, double y, double z);
void matrix4_multiply(matrix4* result, matrix4* a, matrix4* b);
void matrix4_multiply_v4(vector4* result, matrix4* matrix, vector4* point);
void matrix4_identity(matrix4* matrix);

#endif
