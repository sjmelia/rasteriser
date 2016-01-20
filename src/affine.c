#include "affine.h"
#include <stdlib.h>

affine* affine_create()
{
    affine* transform = (affine*)malloc(sizeof(affine));
    transform->transform_matrix = matrix4_create();
    transform->translate_matrix = matrix4_create();
    affine_identity(transform);
    return transform;
}

void affine_identity(affine* affine)
{
    matrix4_identity(affine->transform_matrix);
    matrix4_identity(affine->translate_matrix);
}

void affine_free(affine* transform)
{
    matrix4_free(transform->transform_matrix);
    matrix4_free(transform->translate_matrix);
    free(transform);
}

void affine_translate(affine* transform, double x, double y, double z, double w)
{
    matrix4_identity(transform->translate_matrix);
    // Get a translation matrix
    matrix4_translate(transform->translate_matrix, x, y, z);

    // Apply the translation
    matrix4_multiply(transform->transform_matrix, transform->transform_matrix, transform->translate_matrix);
}

void affine_rotate(affine* transform, double angle, double x, double y, double z, double w)
{
    matrix4_identity(transform->translate_matrix);
    // what about w?
    matrix4_rotation(transform->translate_matrix, angle, x, y, z);

    // Apply the translation
    matrix4_multiply(transform->transform_matrix, transform->transform_matrix, transform->translate_matrix);
}

void affine_apply(affine* transform, vector4* result, vector4* point)
{
    matrix4_multiply_v4(result, transform->transform_matrix, point);
}

