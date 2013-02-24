#include <stdlib.h>
#include "triangle.h"

triangle* triangle_default()
{
    triangle* tri = (triangle*)malloc(sizeof(triangle));
    tri->a = vector4_create(1,1,1,1);
    tri->b = vector4_create(1,1,1,1);
    tri->c = vector4_create(1,1,1,1);
    return tri;
}

triangle* triangle_create(
        double ax, double ay, double az,
        double bx, double by, double bz,
        double cx, double cy, double cz)
{
    triangle* tri = (triangle*)malloc(sizeof(triangle));
    tri->a = vector4_create(ax, ay, az, 1);
    tri->b = vector4_create(bx, by, bz, 1);
    tri->c = vector4_create(cx, cy, cz, 1);
    return tri;
}

void triangle_free(triangle* tri)
{
    vector4_free(tri->a);
    vector4_free(tri->b);
    vector4_free(tri->c);
    free(tri);
}
