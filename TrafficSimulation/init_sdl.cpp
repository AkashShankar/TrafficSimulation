#include "pch.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "init_sdl.h"

void init_sdl()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
    IMG_Init(img_flags);

    TTF_Init();
}

void quit_sdl()
{
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}