#include "rasteriser.h"
#include "vector4.h"
#include <stdlib.h>

rasteriser* rasteriser_create()
{
    rasteriser* rast = (rasteriser*)malloc(sizeof(rasteriser));
    rast->modelMatrix = matrix4_create();
    rast->projectionMatrix = matrix4_create();
    return rast;
}

void rasteriser_free(rasteriser* rast)
{
    matrix4_free(rast->modelMatrix);
    matrix4_free(rast->projectionMatrix);
    free(rast);
}

void rasteriser_translate(rasteriser* rast, double x, double y, double z, double w)
{
    // Get a translation matrix
    matrix4* translate = matrix4_create();
    matrix4_translate(translate, x, y, z);

    // Apply the translation
    matrix4_multiply(rast->modelMatrix, rast->modelMatrix, translate);

    // Free and swap
    matrix4_free(translate);
}

void rasteriser_rotate(rasteriser* rast, double angle, double x, double y, double z, double w)
{
    // Get a translation matrix
    matrix4* translate = matrix4_create();
    // what about w?
    matrix4_rotation(translate, angle, x, y, z);

    // Apply the translation
    matrix4_multiply(rast->modelMatrix, rast->modelMatrix, translate);

    // Free and swap
    matrix4_free(translate);
}

void rasteriser_transform(rasteriser* rast, vector4* result, vector4* point)
{
    matrix4_multiply_v4(result, rast->modelMatrix, point);
    matrix4_multiply_v4(result, rast->projectionMatrix, result);
    vector4_norm(result, result);
    result->x = (result->x + 1) * (640 / 2);
    result->y = (result->y + 1) * (480 / 2);
}


