#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>
#include <iostream>

struct Image {
    SDL_Texture *tex = nullptr;

    void init(std::string f_name, SDL_Renderer *ren);
    void destroy();
};

#endif