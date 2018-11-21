#include "pch.h"
#include "text.h"

#include <SDL_ttf.h>
#include <iostream>

void Text::init(SDL_Renderer *ren, std::string font_name, std::string text, int size)
{
    TTF_Font *font = TTF_OpenFont(font_name.c_str(), size);

    if(!font)
    {
        std::cout << "Text::init() : " << TTF_GetError() << std::endl;
        std::cout << "text is: " << text << std::endl;
    }

    if(tex)
    {
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }

    SDL_Surface *tmp_sur = TTF_RenderText_Solid(font, text.c_str(), col);
    tex = SDL_CreateTextureFromSurface(ren, tmp_sur);

    SDL_FreeSurface(tmp_sur);
    TTF_CloseFont(font);
}

void Text::destroy()
{
    if (tex)
        SDL_DestroyTexture(tex);
}