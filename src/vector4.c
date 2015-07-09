#include <stdlib.h>
#include <math.h>
#include "vector4.h"

vector4* vector4_create(double x, double y, double z, double w)
{
    vector4* vect = (vector4*)malloc(sizeof(vector4));
    vect->x = x;
    vect->y = y;
    vect->z = z;
    vect->w = w;
    return vect;
}

double vector4_length(vector4* target)
{
    double length = (target->x * target->x)
                    + (target->y * target->y)
                    + (target->z * target->z);
                    //+ (target->w * target->w);
    return sqrt(length);
}

void vector4_norm(vector4* result, vector4* target)
{
    double length = vector4_length(target);
    result->x = target->x / length;
    result->y = target->y / length;
    result->z = target->z / length;
}

void vector4_free(vector4* vect)
{
    free(vect);
}
