#ifndef RASTERISER_H
#define RASTERISER_H

#include "matrix4.h"

typedef struct rasteriser
{
    matrix4* modelMatrix;
    matrix4* projectionMatrix;
} rasteriser;

rasteriser* rasteriser_create();
void rasteriser_free(rasteriser* rast);
void rasteriser_translate(rasteriser* rast, double x, double y, double z, double w);
void rasteriser_rotate(rasteriser* rast, double angle, double x, double y, double z, double w);

#endif
