#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <stdio.h>
#include "vector4.h"
#include "matrix4.h"
#include "projection.h" 
#include "triangle.h"
#include "rasteriser.h"
#include "ray.h"

triangle* front;
triangle* back;
triangle* left;
triangle* right;
triangle* bota;
triangle* botb;

rasteriser* rast;
ray* r;

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

void raytrace_scene(SDL_Surface* screen)
{
    // surface is already locked
    unsigned int* pixels = (unsigned int*)screen->pixels;
    unsigned int color = SDL_MapRGB(screen->format, 100,125,150);
    // for each pixel, project a line from the camera
    // to the defined plane
    int x,y;
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
            vector4_minus(r->direction, pixelv, r->origin); //, pixelv);

            if (x == 320 && y == 240)
            {
                printf("Fired a ray from <%f, %f, %f, %f> to <%f, %f, %f, %f> in direction <%f, %f, %f, %f>\n",
                        r->origin->x,
                        r->origin->y,
                        r->origin->z,
                        r->origin->w,
                        pixelv->x,
                        pixelv->y,
                        pixelv->z,
                        pixelv->w,
                        r->direction->x,
                        r->direction->y,
                        r->direction->z,
                        r->direction->w);
            }
            
            if (ray_intersects_tri(r, front))
            {
                pixels[(y * screen->w) + x] = color;
            }
         }
    }

    vector4_free(pixelv);
}

void init()
{
    r = ray_create(0, 0, 0, 0, 0, 0, 0, 0);
    r->origin->x = 0;
    r->origin->y = 0;
    r->origin->z = -2;
    r->origin->w = 0;

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
    raytrace_scene(screen);
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
