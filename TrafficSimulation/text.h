#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <string>

struct Text
{
    SDL_Texture *tex = nullptr;
    SDL_Color col { 255, 0, 0, 255 }; // manual set.

    void init(SDL_Renderer *ren, std::string font_name, std::string text, int size);
    void destroy();
};

#endif