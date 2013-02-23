#include <stdio.h>
//#include "../lib/geom/src/vector3.h"
#include "vector3.h"
#include "matrix4.h"
#include "projection.h" 
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

matrix4* modelMatrix;
matrix4* perspectiveMatrix;

void init()
{
    modelMatrix = matrix4_create();
    perspectiveMatrix = matrix4_create();
    perspective(perspectiveMatrix, 45.0, (640 / 480), 0.1, 100);
}

void apply(vector3* point)
{
    vector3* result = vector3_create(0,0,0);
    matrix4_multiply_v3(result, modelMatrix, point);

    // swap back into point
    matrix4_multiply_v3(point, perspectiveMatrix, result);
    
    // normalises into result 
    vector3_norm(result, point);
    point->x = result->x;
    point->y = result->y;
    point->z = result->z;
    vector3_free(result);

    // viewport bit (?)
    point->x = (point->x + 1) * (640 / 2);
    point->y = (point->y + 1) * (480 / 2);

    // invert y (?)
    //point->y = 480 - point->y;
}

void render_scene(SDL_Surface* screen)
{
    matrix4_identity(modelMatrix);
    
    // Get a translation matrix
    matrix4* translate = matrix4_create();
    vector3* translation = vector3_create(-1.5, 0.0, 6.0);
    matrix4_translation(translate, translation);
    vector3_free(translation);

    // Apply the translation
    matrix4* result = matrix4_create();
    matrix4_multiply(result, modelMatrix, translate);

    // Free and swap
    matrix4_free(modelMatrix);
    matrix4_free(translate);
    printf("Resulta: %f", result->a4);
    modelMatrix = result;
    printf(" model: %f\n", modelMatrix->a4);


    // Triangle
    vector3* top = vector3_create(0.0, 1.0, 0.0);
    vector3* fbl = vector3_create(-1.0, -1.0, 1.0);
    vector3* fbr = vector3_create(1.0, -1.0, 1.0);
    vector3* bbl = vector3_create(-1.0, -1.0, -1.0);
    vector3* bbr = vector3_create(1.0, -1.0, -1.0);

    apply(top);
    apply(fbl);
    apply(fbr);
    apply(bbl);
    apply(bbr);

    printf("Top %f:%f Fbl %f:%f Fbr %f:%f\n", top->x, top->y, fbl->x, fbl->y, fbr->x, fbr->y);

    // Draw front triangle
    lineRGBA(screen, top->x, top->y, fbl->x, fbl->y, 255, 0, 0, 255);
    lineRGBA(screen, fbl->x, fbl->y, fbr->x, fbr->y, 255, 0, 0, 255);
    lineRGBA(screen, fbr->x, fbr->y, top->x, top->y, 255, 0, 0, 255);
    
    // Back
    lineRGBA(screen, top->x, top->y, bbl->x, bbl->y, 0, 255, 0, 255);
    lineRGBA(screen, bbl->x, bbl->y, bbr->x, bbr->y, 0, 255, 0, 255);
    lineRGBA(screen, bbr->x, bbr->y, top->x, top->y, 0, 255, 0, 255);

    // Base
    lineRGBA(screen, bbl->x, bbl->y, fbl->x, fbl->y, 0, 0, 255, 255);
    lineRGBA(screen, bbr->x, bbr->y, fbr->x, fbr->y, 0, 0, 255, 255);

    vector3_free(top);
    vector3_free(fbl);
    vector3_free(fbr);
    vector3_free(bbl);
    vector3_free(bbr);
    
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

    vector3 v1;
    vector3 v2;
    v1.x = 2;
    v2.x = 2;
    v1.y = 2;
    v2.y = 2;
    v1.z = 2;
    v2.z = 2;
    vector3 result;
    vector3_sum(&result, &v1, &v2);
    printf("Hello World! %f\n",result.x);
    SDL_FreeSurface(screen);
}
