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
void vector4_multiply(vector4* result, vector4* left, double right);
void vector4_cross(vector4* result, vector4* left, vector4* right);
double vector4_dot(vector4* left, vector4* right);
void vector4_plus(vector4* result, vector4* left, vector4* right);
void vector4_minus(vector4* result, vector4* left, vector4* right);
double vector4_length(vector4* vector);
void vector4_free(vector4* vect);

#endif
