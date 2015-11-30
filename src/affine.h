#ifndef AFFINE_H
#define AFFINE_H

#include "matrix4.h"
#include "vector4.h"

typedef struct affine
{
    matrix4* transform_matrix;

    // temporary matrix for translations
    matrix4* translate_matrix;
} affine;

affine* affine_create();
void affine_free(affine* transform);
void affine_translate(affine* transform, double x, double y, double z, double w);
void affine_rotate(affine* transform, double angle, double x, double y, double z, double w);
void affine_apply(affine* transform, vector4* result, vector4* point);
#endif
