#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <stdio.h>
#include "vector4.h"
#include "matrix4.h"
#include "projection.h" 
#include "triangle.h"
#include "rasteriser.h"

triangle* front;
triangle* back;
triangle* left;
triangle* right;
triangle* bota;
triangle* botb;

typedef struct raytracer
{
    vector4* camera;
    double focal_length;
} raytracer;

rasteriser* rast;
raytracer* ray;

void render_triangle(SDL_Surface* screen, triangle* tri, int r, int g, int b)
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


void render_scene(SDL_Surface* screen)
{
    //rasteriser_rotate(rast, 0.5, 0.0, 1.0, 0.0, 1.0); 

    render_triangle(screen, front, 255, 0, 0);
    //render_triangle(screen, back, 0, 255, 0);
    //render_triangle(screen, left, 0, 0, 255);
    //render_triangle(screen, right, 255, 255, 0);
    //render_triangle(screen, bota, 255, 0, 255);
    //render_triangle(screen, botb, 0, 255, 255);
}

#define EPSILON 0.000001
vector4* e1;
vector4* e2;
vector4* p;
vector4* q;
vector4* t;
 
int ray_intersects_tri(vector4* origin, vector4* direction, triangle* tri)
{
    //return 1;
    double det, inv_det, u, v;
    double t2;

    vector4_minus(e1, tri->b, tri->a);
    vector4_minus(e2, tri->c, tri->a);

    vector4_cross(p, direction, e2);
    det = vector4_dot(e1, p);
    if (det > -EPSILON && det < EPSILON) return 0;
    inv_det = 1.0f / det;
    vector4_minus(t, origin, tri->a);
    u = vector4_dot(t, p) * inv_det;
    if (u < 0.0f || u > 1.0f) return 0;
    vector4_cross(q, t, e1);
    v = vector4_dot(direction, q) * inv_det;
    if (v < 0.0f || u + v > 1.0f) return 0;
    t2 = vector4_dot(e2, q) * inv_det;

    if (t2 > EPSILON)
    {
        // *out = t2;
        return 1;
    }

    return 0;
}

void raytrace_scene(raytracer* raytracer, SDL_Surface* screen)
{
    // surface is already locked
    unsigned int* pixels = (unsigned int*)screen->pixels;
    unsigned int color = SDL_MapRGB(screen->format, 100,125,150);
    // for each pixel, project a line from the camera
    // to the defined plane
    int x,y;
    vector4* direction = vector4_create(0, 0, 0, 0);
    vector4* pixelv = vector4_create(0, 0, 0, 0);
    for (y = 0; y < screen->h; y++)
    {
        pixelv->y = y-240;
        pixelv->y *= 0.005;
        for (x = 0; x < screen->w; x++)
        {
            // get the direction vector for this line
            pixelv->x = x-320;
            pixelv->x *= 0.005;
            vector4_minus(direction, pixelv, raytracer->camera); //, pixelv);

            if (x == 320 && y == 240)
            {
                printf("Fired a ray from <%f, %f, %f, %f> to <%f, %f, %f, %f> in direction <%f, %f, %f, %f>\n",
                        raytracer->camera->x,
                        raytracer->camera->y,
                        raytracer->camera->z,
                        raytracer->camera->w,
                        pixelv->x,
                        pixelv->y,
                        pixelv->z,
                        pixelv->w,
                        direction->x,
                        direction->y,
                        direction->z,
                        direction->w);
            }
            
            if (ray_intersects_tri(raytracer->camera, direction, front))
            {
                pixels[(y * screen->w) + x] = color;
            }
         }
    }

    vector4_free(pixelv);
    vector4_free(direction);
}

void init()
{
    e1 = vector4_create(0, 0, 0, 0);
    e2 = vector4_create(0, 0, 0, 0);
    p = vector4_create(0, 0, 0, 0);
    q = vector4_create(0, 0, 0, 0);
    t = vector4_create(0, 0, 0, 0);
 
    ray = (raytracer*)malloc(sizeof(raytracer));
    ray->camera = vector4_create(0, 0, -2, 0);
    ray->focal_length = 1;

    rast = rasteriser_create();
    // camera is between 1.45 and 3 from the object?
    perspective(rast->projectionMatrix, 45.0, (640 / 480), 0.1, 100);
    matrix4_identity(rast->modelMatrix);
    //rasteriser_translate(rast, 0.0, 0.0, 3.0, 1.0);    
    
    back = triangle_create(
            0.0, -1.0, 0.0,
            -1.0, 1.0, 1.0,
            1.0, 1.0, 1.0);
    front = triangle_create(
            0.0, -1.0, 0.0,
            -1.0, 1.0, -1.0,
            1.0, 1.0, -1.0);
    left = triangle_create(
            0.0, -1.0, 0.0,
            -1.0, 1.0, -1.0,
            -1.0, 1.0, 1.0);
    right = triangle_create(
            0.0, -1.0, 0.0,
            1.0, 1.0, -1.0,
            1.0, 1.0, 1.0);

    bota = triangle_create(
            -1.0, 1.0, 1.0,
            1.0, 1.0, 1.0,
            -1.0, 1.0, -1.0);
    botb = triangle_create(
            -1.0, 1.0, -1.0,
            1.0, 1.0, -1.0,
            1.0, 1.0, 1.0);
}


void render(SDL_Surface* screen)
{
    SDL_LockSurface(screen);
    unsigned int* pixels = (unsigned int*)screen->pixels;
    int x,y;
    unsigned int color = SDL_MapRGB(screen->format, 0,0,0);
    for (y = 0; y < screen->h; y++)
    {
        for (x = 0; x < screen->w; x++)
        {
            pixels[(y * screen->w) + x] = color;
        }
    }
    raytrace_scene(ray, screen);
    render_scene(screen);
    SDL_UnlockSurface(screen);
    SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
}


int main()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
    init();
    SDL_Event event;
    int numFrames = 0;
    //int startTime = SDL_GetTicks();
    while(1)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                printf("Got Quit\n");
                break;
            }
        }
        render(screen);
        ++numFrames;

        //float fps = (numFrames/(float)(SDL_GetTicks() - startTime))*1000;
        //printf("FPS: %f\n",fps);
    }

   SDL_FreeSurface(screen);
   return 0;
}
