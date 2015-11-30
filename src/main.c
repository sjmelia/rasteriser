#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <stdio.h>
#include "vector4.h"
#include "matrix4.h"
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


void raytrace_scene(SDL_Surface* screen)
{
    // surface is already locked
    unsigned int* pixels = (unsigned int*)screen->pixels;
    unsigned int color = SDL_MapRGB(screen->format, 100,125,150);
    // for each pixel, project a line from the camera
    // to the defined plane
    int x,y;
    vector4* pixelv = vector4_create(0, 0, 0, 0);
    double fovx = 45.0;
    double fovy = (480.0/640.0) * fovx;
    for (y = 0.0; y < screen->h; y++)
    {
        
        //pixelv->y = y-240;
        //pixelv->y *= 0.005;
        double dy = y;
        pixelv->y = (((2.0 * dy) - 480.0) / 480.0) * tan(fovy);
        for (x = 0.0; x < screen->w; x++)
        {
            // get the direction vector for this line
            //pixelv->x = x-320;
            //pixelv->x *= 0.005;
            double dx = x;
            pixelv->x = (((2.0 * dx) - 640.0) / 640.0) * tan(fovx);
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

affine* transform;

void init()
{
    transform = affine_create();
    affine_rotate(transform, 0.5, 0.0, 1.0, 0.0, 1.0); 

    r = ray_create(0, 0, 0, 0, 0, 0, 0, 0);
    r->origin->x = 0;
    r->origin->y = 0;
    r->origin->z = -1;
    r->origin->w = 0;

    rast = rasteriser_create();
    // camera is between 1.45 and 3 from the object?
    rasteriser_perspective(rast, 45.0, (640 / 480), 0.1, 100);
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
    //rasteriser_render(rast, screen);
    rasteriser_render_triangle(rast, screen, front, 255, 0, 0);
    

    affine_apply(transform, front->a, front->a);
    affine_apply(transform, front->b, front->b);
    affine_apply(transform, front->c, front->c);

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
