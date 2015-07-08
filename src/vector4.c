#include <stdlib.h>
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

void vector4_free(vector4* vect)
{
    free(vect);
}
