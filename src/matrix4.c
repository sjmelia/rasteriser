#include <stdlib.h>
#include <math.h>
#include "matrix4.h"

matrix4* matrix4_create()
{
    matrix4* matrix = (matrix4*)malloc(sizeof(matrix4));
    matrix4_identity(matrix);
    return matrix;
}

void matrix4_clone(matrix4* a, matrix4* b)
{
    a->a1 = b->a1; a->a2 = b->a2; a->a3 = b->a3; a->a4 = b->a4;
    a->b1 = b->b1; a->b2 = b->b2; a->b3 = b->b3; a->b4 = b->b4;
    a->c1 = b->c1; a->c2 = b->c2; a->c3 = b->c3; a->c4 = b->c4;
    a->d1 = b->d1; a->d2 = b->d2; a->d3 = b->d3; a->d4 = b->d4;
}

void matrix4_free(matrix4* matrix)
{
    free(matrix);
}

void matrix4_multiply(matrix4* c, matrix4* a, matrix4* b)
{
    double a1 = a->a1 * b->a1 + a->a2 * b->b1 + a->a3 * b->c1 + a->a4 * b->d1;
    double a2 = a->a1 * b->a2 + a->a2 * b->b2 + a->a3 * b->c2 + a->a4 * b->d2;
    double a3 = a->a1 * b->a3 + a->a2 * b->b3 + a->a3 * b->c3 + a->a4 * b->d3;
    double a4 = a->a1 * b->a4 + a->a2 * b->b4 + a->a3 * b->c4 + a->a4 * b->d4;

    double b1 = a->b1 * b->a1 + a->b2 * b->b1 + a->b3 * b->c1 + a->b4 * b->d1;
    double b2 = a->b1 * b->a2 + a->b2 * b->b2 + a->b3 * b->c2 + a->b4 * b->d2;
    double b3 = a->b1 * b->a3 + a->b2 * b->b3 + a->b3 * b->c3 + a->b4 * b->d3;
    double b4 = a->b1 * b->a4 + a->b2 * b->b4 + a->b3 * b->c4 + a->b4 * b->d4;

    double c1 = a->c1 * b->a1 + a->c2 * b->b1 + a->c3 * b->c1 + a->c4 * b->d1;
    double c2 = a->c1 * b->a2 + a->c2 * b->b2 + a->c3 * b->c2 + a->c4 * b->d2;
    double c3 = a->c1 * b->a3 + a->c2 * b->b3 + a->c3 * b->c3 + a->c4 * b->d3;
    double c4 = a->c1 * b->a4 + a->c2 * b->b4 + a->c3 * b->c4 + a->c4 * b->d4;
    
    double d1 = a->d1 * b->a1 + a->d2 * b->b1 + a->d3 * b->c1 + a->d4 * b->d1;
    double d2 = a->d1 * b->a2 + a->d2 * b->b2 + a->d3 * b->c2 + a->d4 * b->d2;
    double d3 = a->d1 * b->a3 + a->d2 * b->b3 + a->d3 * b->c3 + a->d4 * b->d3;
    double d4 = a->d1 * b->a4 + a->d2 * b->b4 + a->d3 * b->c4 + a->d4 * b->d4;

    c->a1 = a1; c->a2 = a2; c->a3 = a3; c->a4 = a4;
    c->b1 = b1; c->b2 = b2; c->b3 = b3; c->b4 = b4;
    c->c1 = c1; c->c2 = c2; c->c3 = c3; c->c4 = c4;
    c->d1 = d1; c->d2 = d2; c->d3 = d3; c->d4 = d4;
}

void matrix4_multiply_v4(vector4* c, matrix4* a, vector4* b)
{
    double x = (a->a1 * b->x) + (a->a2 * b->y) + (a->a3 * b->z) + (a->a4 * b->w);
    double y = a->b1 * b->x + a->b2 * b->y + a->b3 * b->z + a->b4 * b->w;
    double z = a->c1 * b->x + a->c2 * b->y + a->c3 * b->z + a->c4 * b->w;
    double w = a->d1 * b->x + a->d2 * b->y + a->d3 * b->z + a->d4 * b->w;
    c->x = x; c->y = y; c->z = z; c->w = w;
}

void matrix4_rotation(matrix4* matrix, double angle, double x, double y, double z)
{
    vector4* v = vector4_create(x, y, z, 0);
    vector4_norm(v, v);
    x = v->x;
    y = v->y;
    z = v->z;
    angle *= M_PI / 180;

    matrix4* m4 = matrix4_create();
    double c = cos(angle);
    double s = sin(angle);

    m4->a1 = x * x * (1 - c) + c;
    m4->a2 = x * y * (1 - c) - z * s;
    m4->a3 = x * z * (1 - c) + y * s;
    m4->a4 = 0;

    m4->b1 = y * x * (1 - c) + z * s;
    m4->b2 = y * y * (1 - c) + c;
    m4->b3 = y * z * (1 - c) - x * s;
    m4->b4 = 0;

    m4->c1 = z * x * (1 - c) - y * s;
    m4->c2 = z * y * (1 - c) + x * s;
    m4->c3 = z * z * (1 - c) + c;
    m4->c4 = 0;

    m4->d1 = 0;
    m4->d2 = 0;
    m4->d3 = 0;
    m4->d4 = 1;

    matrix4* temp = matrix4_create();
    matrix4_multiply(temp, matrix, m4);
    matrix4_clone(matrix, temp);
    matrix4_free(temp);
    matrix4_free(m4);
}

void matrix4_translate(matrix4* matrix, double x, double y, double z)
{
    matrix4* m = matrix4_create();
    matrix4_identity(m);
    m->a4 = x;
    m->b4 = y;
    m->c4 = z;
    m->d4 = 1;
    matrix4_multiply(matrix, matrix, m);
    matrix4_free(m);
}

void matrix4_identity(matrix4* matrix)
{
    matrix->a1 = 1;
    matrix->a2 = 0;
    matrix->a3 = 0;
    matrix->a4 = 0;
    matrix->b1 = 0;
    matrix->b2 = 1;
    matrix->b3 = 0;
    matrix->b4 = 0;
    matrix->c1 = 0;
    matrix->c2 = 0;
    matrix->c3 = 1;
    matrix->c4 = 0;
    matrix->d1 = 0;
    matrix->d2 = 0;
    matrix->d3 = 0;
    matrix->d4 = 1;
}
