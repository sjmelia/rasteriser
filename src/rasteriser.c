#include "rasteriser.h"
#include "vector4.h"
#include "triangle.h"
#include "affine.h"
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL_gfxPrimitives.h>

rasteriser* rasteriser_create()
{
    rasteriser* rast = (rasteriser*)malloc(sizeof(rasteriser));
    rast->model_affine = affine_create();
    rast->projection_matrix = matrix4_create();
    return rast;
}

void rasteriser_free(rasteriser* rast)
{
    affine_free(rast->model_affine);
    matrix4_free(rast->projection_matrix);
    free(rast);
}

void frustum(matrix4* m, double left, double right, double bottom, double top, double znear, double zfar)
{
    m->a1 = (2 * znear) / (right - left);
    m->a3 = (right + left) / (right - left);
    m->b2 = (2 * znear) / (top - bottom);
    m->b3 = (top + bottom) / (top - bottom);
    m->c3 = - ((zfar + znear) / (zfar - znear));
    m->c4 = - ((2 * zfar * znear) / (zfar - znear));
    m->d3 = -1;
    // undo identity
    m->d4 = 0;
}

void rasteriser_perspective(rasteriser* rast, double fov, double aspect, double nearplane, double farplane)
{
    double xmin, xmax, ymin, ymax;
    ymax = nearplane * (double)tan(fov * M_PI / 360);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    frustum(rast->projection_matrix, xmin, xmax, ymin, ymax, nearplane, farplane);
}

void rasteriser_transform(rasteriser* rast, vector4* result, vector4* point)
{
    affine_apply(rast->model_affine, result, point);
    matrix4_multiply_v4(result, rast->projection_matrix, result);
//    vector4_norm(result, result);

    // http://stackoverflow.com/questions/3792481/how-to-get-screen-coordinates-from-a-3d-point-opengl
    result->x = result->x / result->w;
    result->y = result->y / result->w;
    result->z = result->z / result->w;

    //result->x = (320 * (result->x + 1)) / 2;
    //result->y = (218.141434 * (result->y + 1)) / 2;
    //result->z = (result->z + 1) / 2;*/
    result->x = (result->x + 1) * (320 / 2);
    result->y = (result->y + 1) * (218.141434 / 2);
    // no idea what this is about
//    result->x = (result->x + 1) * (640 / 2);
//    result->y = (result->y + 1) * (480 / 2);
    //result->x = result->x * 640;
    //result->y = result->y * 480;
    //result->y = 480 - result->y;

}

void rasteriser_render_triangle(rasteriser* rast, SDL_Surface* screen, triangle* tri, int r, int g, int b)
{
    vector4* transformeda = vector4_create(1,1,1,1);
    vector4* transformedb = vector4_create(1,1,1,1);
    vector4* transformedc = vector4_create(1,1,1,1);

    rasteriser_transform(rast, transformeda, tri->a);
    rasteriser_transform(rast, transformedb, tri->b);
    rasteriser_transform(rast, transformedc, tri->c);

    lineRGBA(screen, transformeda->x, transformeda->y, transformedb->x, transformedb->y, r, g, b, 255);
    lineRGBA(screen, transformedb->x, transformedb->y, transformedc->x, transformedc->y, r, g, b, 255);
    lineRGBA(screen, transformedc->x, transformedc->y, transformeda->x, transformeda->y, r, g, b, 255);

    vector4_free(transformeda);
    vector4_free(transformedb);
    vector4_free(transformedc);
}

void rasteriser_render(rasteriser* rast, SDL_Surface* screen)
{

    //render_triangle(rast, screen, front, 255, 0, 0);
    //render_triangle(screen, back, 0, 255, 0);
    //render_triangle(screen, left, 0, 0, 255);
    //render_triangle(screen, right, 255, 255, 0);
    //render_triangle(screen, bota, 255, 0, 255);
    //render_triangle(screen, botb, 0, 255, 255);
}

