#include "pch.h"
#include "image.h."

#include <SDL_image.h>

void Image::init(std::string f_name, SDL_Renderer *ren)
{
    SDL_Surface *tmp = IMG_Load(f_name.c_str());
    if (!tmp)
    {
        std::cout << IMG_GetError() << std::endl;
    }
    tex = SDL_CreateTextureFromSurface(ren, tmp);

    SDL_FreeSurface(tmp);
}

void Image::destroy()
{
    SDL_DestroyTexture(tex);
}