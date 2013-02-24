#include <stdio.h>
//#include "../lib/geom/src/vector3.h"
#include "vector4.h"
#include "matrix4.h"
#include "projection.h" 
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "triangle.h"
#include "rasteriser.h"

triangle* front;
rasteriser* rast;

/*void apply(vector4* point)
{
    //vector4* result = vector4_create(0,0,0,0);
    matrix4_multiply_v4(apply_result, modelMatrix, point);

    // swap back into point
    matrix4_multiply_v4(point, perspectiveMatrix, apply_result);
    
    // normalises into result 
    vector4_norm(apply_result, point);
    point->x = apply_result->x;
    point->y = apply_result->y;
    point->z = apply_result->z;
    point->w = apply_result->w;
    //vector4_free(result);

    // viewport bit (?)
    point->x = (point->x + 1) * (640 / 2);
    point->y = (point->y + 1) * (480 / 2);

    // invert y (?)
    //point->y = 480 - point->y;
}*/

void render_triangle(SDL_Surface* screen, triangle* tri)
{
    vector4* transformeda = vector4_create(1,1,1,1);
    vector4* transformedb = vector4_create(1,1,1,1);
    vector4* transformedc = vector4_create(1,1,1,1);

    rasteriser_transform(rast, transformeda, tri->a);
    rasteriser_transform(rast, transformedb, tri->b);
    rasteriser_transform(rast, transformedc, tri->c);

    lineRGBA(screen, transformeda->x, transformeda->y, transformedb->x, transformedb->y, 255, 255, 255, 255);
    lineRGBA(screen, transformedb->x, transformedb->y, transformedc->x, transformedc->y, 255, 255, 255, 255);
    lineRGBA(screen, transformedc->x, transformedc->y, transformeda->x, transformeda->y, 255, 255, 255, 255);

    vector4_free(transformeda);
    vector4_free(transformedb);
    vector4_free(transformedc);
}


void render_scene(SDL_Surface* screen)
{
    rasteriser_rotate(rast, 1.0, 0.0, 1.0, 0.0, 1.0); 

    render_triangle(screen, front);

    // Triangle
/*    vector4* top = vector4_create(0.0, 1.0, 0.0, 1.0);
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
  */  
}
void init()
{
    rast = rasteriser_create();
    perspective(rast->projectionMatrix, 45.0, (640 / 480), 0.1, 100);
    matrix4_identity(rast->modelMatrix);
    rasteriser_translate(rast, 0.0, 0.0, 3.0, 1.0);    
    
//    apply_result = vector4_create(0,0,0,0);
    front = triangle_create(
            0.0, 1.0, 0.0,
            -1.0, -1.0, 1.0,
            1.0, -1.0, 1.0);
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
    int numFrames = 0;
    int startTime = SDL_GetTicks();
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
        ++numFrames;

        float fps = (numFrames/(float)(SDL_GetTicks() - startTime))*1000;
        printf("FPS: %f\n",fps);
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
