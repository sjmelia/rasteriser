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

unsigned int black;
unsigned int green;
unsigned int red;

//affine* transform;

void raytrace_scene(SDL_Surface* screen)
{
    // surface is already locked
    unsigned int* pixels = (unsigned int*)screen->pixels;
    // for each pixel, project a line from the camera
    // to the defined plane
    int x,y;
    vector4* pixelv = vector4_create(0, 0, 0, 0);
    double fovx = 60.0;
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

            /*if (x == 320 && y == 240)
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
            }*/
            double t2; 
            if (ray_intersects_tri(r, front, &t2))
            {
                //int col2 = t2 * 255;
                //unsigned int color2 = SDL_MapRGB(screen->format, 0,col2,0);
                int cy = 479 - y;
                //printf("y: %d cy: %d t2:%f\n", y, cy, t2);
                pixels[(cy * screen->w) + x] = red;
            }
            
            /*if (ray_intersects_tri(r, left, &t2))
            {
                int col2 = t2 * 255;
                unsigned int color2 = SDL_MapRGB(screen->format, col2,0,0);
                int cy = 479 - y;
                //printf("y: %d cy: %d t2:%f\n", y, cy, t2);
                pixels[(cy * screen->w) + x] = green;
            }*/
         }
    }

    vector4_free(pixelv);
}

void init(SDL_Surface* screen)
{
    black = SDL_MapRGB(screen->format, 0,0,0);
    green = SDL_MapRGB(screen->format, 0,255,0);
    red = SDL_MapRGB(screen->format, 255,0,0);

    //transform = affine_create();

    r = ray_create(0, 0, 0, 0, 0, 0, 0, 0);
    r->origin->x = 0;
    r->origin->y = 0;
    r->origin->z = -8;
    r->origin->w = 0;

    rast = rasteriser_create(640, 480);
    float width = 640.0;
    float height = 480.0;
    rasteriser_perspective(rast, 60.0, (width / height), 1.0, 1024.0);
    
    //affine_rotate(rast->model_affine, 25, 0.0, 1.0, 0.0, 1.0); 
    back = triangle_create(
            0.0, 1.0, -4.0,
            1.0, -1.0, -5.0,
            -1.0, -1.0, -5.0);
    front = triangle_create(
            -1.0, -1.0, -3.0,
            1.0, -1.0, -3.0,
            0.0, 1.0, -4.0);
    left = triangle_create(
            -1.0, -1.0, -5.0,
            -1.0, -1.0, -3.0,
            0.0, 1.0, -4.0);
    right = triangle_create(
            0.0, 1.0, -4.0,
            1.0, -1.0, -3.0,
            1.0, -1.0, -5.0);

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
    for (y = 0; y < screen->h; y++)
    {
        for (x = 0; x < screen->w; x++)
        {
            pixels[(y * screen->w) + x] = black;
        }
    }

    static float angle = 0.0f; 
    affine_identity(rast->model_affine);
    affine_translate(rast->model_affine, 0.0, 0.0, -4.0, 0.0); 
    affine_rotate(rast->model_affine, angle, 0.0, 1.0, 0.0, 0.0); 
    affine_translate(rast->model_affine, 0.0, 0.0, 4.0, 0.0);
    if (++angle > 360.0f)
    {
        angle = 0.0f;
    }

    //raytrace_scene(screen);
    rasteriser_render_triangle(rast, screen, front, 255, 0, 0);
    rasteriser_render_triangle(rast, screen, back, 0, 255, 0);
    rasteriser_render_triangle(rast, screen, left, 0, 0, 255);
    rasteriser_render_triangle(rast, screen, right, 255, 255, 255);
/*
    affine_apply(transform, front->a, front->a);
    affine_apply(transform, front->b, front->b);
    affine_apply(transform, front->c, front->c);
  */ 
    SDL_UnlockSurface(screen);
    SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
}


int main()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
    init(screen);
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
