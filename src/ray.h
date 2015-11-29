#ifndef RAY_H
#define RAY_H

#include "triangle.h"

struct ray_private;

typedef struct ray
{
    vector4* origin;
    vector4* direction;
    struct ray_private* p;
} ray;

ray* ray_create(double ox, double oy, double oz, double ow, double dx, double dy, double dz, double dw);
void ray_free(ray* r);
int ray_intersects_tri(ray* r, triangle* tri);

#endif

