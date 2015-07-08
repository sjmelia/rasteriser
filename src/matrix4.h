#ifndef MATRIX4_H
#define MATRIX4_H

typedef struct matrix4
{
} matrix4;

matrix4* matrix4_create();
void matrix4_free(matrix4* matrix);
void matrix4_rotation(matrix4* matrix, double angle, vector4* translation);
void matrix4_multiply_v4(vector4* result, matrix4* matrix, vector4* point);

#endif
