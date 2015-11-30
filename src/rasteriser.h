#ifndef RASTERISER_H
#define RASTERISER_H

#include <SDL/SDL.h>
#include "matrix4.h"
#include "vector4.h"
#include "affine.h"
#include "triangle.h"

typedef struct rasteriser
{
    affine* model_affine;
    matrix4* projection_matrix;
} rasteriser;

rasteriser* rasteriser_create();
void rasteriser_free(rasteriser* rast);
void rasteriser_render(rasteriser* rast, SDL_Surface* screen);
void rasteriser_render_triangle(rasteriser* rast, SDL_Surface* screen, triangle* tri, int r, int g, int b);
void rasteriser_perspective(rasteriser* rast, double fov, double aspect, double nearplane, double farplane);
#endif
