#include <stdlib.h>
#include <math.h>
#include "vector4.h"
#include "ray.h"

#define EPSILON 0.000001

struct ray_private {
    // temporary vars for calculating
    // intersections; we don't want to
    // malloc them each time.
    //
    // this means that intersection calls
    // are not threadsafe for a single ray
    // but are for different rays
    vector4* e1;
    vector4* e2;
    vector4* p;
    vector4* q;
    vector4* t;
};

ray* ray_create(double ox, double oy, double oz, double ow, double dx, double dy, double dz, double dw)
{
    ray* r = (ray*)malloc(sizeof(ray));
    r->origin = vector4_create(ox, oy, oz, ow);
    r->direction = vector4_create(dx, dy, dz, dw);
    r->p = malloc(sizeof(struct ray_private));
    r->p->e1 = vector4_create(0, 0, 0, 0);
    r->p->e2 = vector4_create(0, 0, 0, 0);
    r->p->p = vector4_create(0, 0, 0, 0);
    r->p->q = vector4_create(0, 0, 0, 0);
    r->p->t = vector4_create(0, 0, 0, 0);
    return r;
}

void ray_free(ray* r)
{
    vector4_free(r->origin);
    vector4_free(r->direction);
    vector4_free(r->p->e1);
    vector4_free(r->p->e2);
    vector4_free(r->p->p);
    vector4_free(r->p->q);
    vector4_free(r->p->t);
    free(r->p);
    free(r);
}

int ray_intersects_tri(ray* r, triangle* tri)
{
    //return 1;
    double det, inv_det, u, v;
    double t2;

    vector4_minus(r->p->e1, tri->b, tri->a);
    vector4_minus(r->p->e2, tri->c, tri->a);

    vector4_cross(r->p->p, r->direction, r->p->e2);
    det = vector4_dot(r->p->e1, r->p->p);
    if (det > -EPSILON && det < EPSILON) return 0;
    inv_det = 1.0f / det;
    vector4_minus(r->p->t, r->origin, tri->a);
    u = vector4_dot(r->p->t, r->p->p) * inv_det;
    if (u < 0.0f || u > 1.0f) return 0;
    vector4_cross(r->p->q, r->p->t, r->p->e1);
    v = vector4_dot(r->direction, r->p->q) * inv_det;
    if (v < 0.0f || u + v > 1.0f) return 0;
    t2 = vector4_dot(r->p->e2, r->p->q) * inv_det;

    if (t2 > EPSILON)
    {
        // *out = t2;
        return 1;
    }

    return 0;
}

