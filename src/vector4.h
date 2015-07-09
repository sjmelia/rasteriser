#ifndef VECTOR4_H
#define VECTOR4_H

typedef struct vector4
{
    double x;
    double y;
    double z;
    double w;
} vector4;

vector4* vector4_create(double x, double y, double z, double w);
void vector4_norm(vector4* result, vector4* target);
double vector4_length(vector4* vector);
void vector4_free(vector4* vect);

#endif
