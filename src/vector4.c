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

void vector4_cross(vector4* result, vector4* left, vector4* right)
{
    result->x = ((left->y * right->z) - (left->z * right->y));
    result->y = -((left->x * right->z) - (left->z * right->x));
    result->z = ((left->x * right->y) - (left->y * right->x));
    result->w = 0;
}

double vector4_dot(vector4* left, vector4* right)
{
    return (left->x * right->x) + (left->y * right->y) + (left->z * right->z) + (left->w * right->w); 
}

void vector4_multiply(vector4* result, vector4* left, double right)
{
    result->x = left->x * right;
    result->y = left->y * right;
    result->z = left->z * right;
    result->w = left->w * right;
}

void vector4_plus(vector4* result, vector4* left, vector4* right)
{
    result->x = left->x + right->x;
    result->y = left->y + right->y;
    result->z = left->z + right->z;
    result->w = left->w + right->w;
}

void vector4_minus(vector4* result, vector4* left, vector4* right)
{
    result->x = left->x - right->x;
    result->y = left->y - right->y;
    result->z = left->z - right->z;
    result->w = left->w - right->w;
}

void vector4_free(vector4* vect)
{
    free(vect);
}
