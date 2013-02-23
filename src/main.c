#include <stdio.h>
//#include "../lib/geom/src/vector3.h"
#include "vector4.h"
#include "matrix4.h"
#include "projection.h" 
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

matrix4* modelMatrix;
matrix4* perspectiveMatrix;

void apply(vector4* point)
{
    vector4* result = vector4_create(0,0,0,0);
    matrix4_multiply_v4(result, modelMatrix, point);

    // swap back into point
    matrix4_multiply_v4(point, perspectiveMatrix, result);
    
    // normalises into result 
    vector4_norm(result, point);
    point->x = result->x;
    point->y = result->y;
    point->z = result->z;
    point->w = result->w;
    vector4_free(result);

    // viewport bit (?)
    point->x = (point->x + 1) * (640 / 2);
    point->y = (point->y + 1) * (480 / 2);

    // invert y (?)
    //point->y = 480 - point->y;
}

void translate(double x, double y, double z, double w)
{
    // Get a translation matrix
    matrix4* translate = matrix4_create();
    vector4* translation = vector4_create(x, y, z, w);
    matrix4_translation(translate, translation);
    vector4_free(translation);

    // Apply the translation
    matrix4* result = matrix4_create();
    matrix4_multiply(result, modelMatrix, translate);

    // Free and swap
    matrix4_free(modelMatrix);
    matrix4_free(translate);
    modelMatrix = result;
}
void rotate(double angle, double x, double y, double z, double w)
{
    // Get a translation matrix
    matrix4* translate = matrix4_create();
    vector4* translation = vector4_create(x, y, z, w);
    matrix4_rotation(translate, angle, translation);
    vector4_free(translation);

    // Apply the translation
    matrix4* result = matrix4_create();
    matrix4_multiply(result, modelMatrix, translate);

    // Free and swap
    matrix4_free(modelMatrix);
    matrix4_free(translate);
    modelMatrix = result;
}


void render_scene(SDL_Surface* screen)
{
    rotate(1.0, 0.0, 1.0, 0.0, 1.0); 

    // Triangle
    vector4* top = vector4_create(0.0, 1.0, 0.0, 1.0);
    vector4* fbl = vector4_create(-1.0, -1.0, 1.0, 1.0);
    vector4* fbr = vector4_create(1.0, -1.0, 1.0, 1.0);
    vector4* bbl = vector4_create(-1.0, -1.0, -1.0, 1.0);
    vector4* bbr = vector4_create(1.0, -1.0, -1.0, 1.0);

    apply(top);
    apply(fbl);
    apply(fbr);
    apply(bbl);
    apply(bbr);

    //printf("Top %f:%f Fbl %f:%f Fbr %f:%f\n", top->x, top->y, fbl->x, fbl->y, fbr->x, fbr->y);

    // Draw front triangle
    lineRGBA(screen, top->x, top->y, fbl->x, fbl->y, 255, 255, 255, 255);
    lineRGBA(screen, fbl->x, fbl->y, fbr->x, fbr->y, 255, 255, 255, 255);
    lineRGBA(screen, fbr->x, fbr->y, top->x, top->y, 255, 255, 255, 255);
    
    // Back
    lineRGBA(screen, top->x, top->y, bbl->x, bbl->y, 255, 255, 255, 255);
    lineRGBA(screen, bbl->x, bbl->y, bbr->x, bbr->y, 255, 255, 255, 255);
    lineRGBA(screen, bbr->x, bbr->y, top->x, top->y, 255, 255, 255, 255);

    // Base
    lineRGBA(screen, bbl->x, bbl->y, fbl->x, fbl->y, 255, 255, 255, 255);
    lineRGBA(screen, bbr->x, bbr->y, fbr->x, fbr->y, 255, 255, 255, 255);

    vector4_free(top);
    vector4_free(fbl);
    vector4_free(fbr);
    vector4_free(bbl);
    vector4_free(bbr);
    
}
void init()
{
    modelMatrix = matrix4_create();
    perspectiveMatrix = matrix4_create();
    perspective(perspectiveMatrix, 45.0, (640 / 480), 0.1, 100);
    matrix4_identity(modelMatrix);
    translate(0.0, 0.0, 3.0, 1.0);    
    
}


void render(SDL_Surface* screen)
{
    SDL_LockSurface(screen);
    unsigned int* pixels = (unsigned int*)screen->pixels;
    int x,y;
    unsigned int color = SDL_MapRGB(screen->format, 0,0,0); //255, 150, 255);
   // printf("Rendering %d:%d %d\n",screen->w, screen->h, sizeof(unsigned int));
    for (y = 0; y < screen->h; y++)
    {
        for (x = 0; x < screen->w; x++)
        {
            //printf("printing %d:%d\n", x, y);
            pixels[(y * screen->w) + x] = color;
        }
    }
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
    while(true)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                printf("GOT QUIT\n");
                break;
            }
        }
        //printf("EVENT\n");
        render(screen);
    }

    vector4 v1;
    vector4 v2;
    v1.x = 2;
    v2.x = 2;
    v1.y = 2;
    v2.y = 2;
    v1.z = 2;
    v2.z = 2;
    vector4 result;
    vector4_sum(&result, &v1, &v2);
    printf("Hello World! %f\n",result.x);
    SDL_FreeSurface(screen);
}
