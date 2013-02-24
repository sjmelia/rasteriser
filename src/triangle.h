#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector4.h"

typedef struct triangle
{
    vector4* a;
    vector4* b;
    vector4* c;
} triangle;

triangle* triangle_default();
triangle* triangle_create(
        double ax, double ay, double az,
        double bx, double by, double bz,
        double cx, double cy, double cz);
void triangle_free(triangle* tri);
#endif
