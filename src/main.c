#include <stdio.h>
//#include "../lib/geom/src/vector3.h"
#include "vector3.h"
#include <SDL/SDL.h>

void render(SDL_Surface* screen)
{
    SDL_LockSurface(screen);
    unsigned int* pixels = (unsigned int*)screen->pixels;
    int x,y;
    unsigned int color = SDL_MapRGB(screen->format, 255, 150, 255);
   // printf("Rendering %d:%d %d\n",screen->w, screen->h, sizeof(unsigned int));
    for (y = 0; y < screen->h; y++)
    {
        for (x = 0; x < screen->w; x++)
        {
            //printf("printing %d:%d\n", x, y);
            pixels[(y * screen->w) + x] = color;
        }
    }
    SDL_UnlockSurface(screen);
    SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

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
