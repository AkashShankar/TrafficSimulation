#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <string>

struct Display {
    SDL_Renderer *ren = nullptr;
    SDL_Window *win = nullptr;

    int win_id = -1;

    int window_width = 0;
    int window_height = 0;

    bool shown = false;

    void init(std::string title, int w, int h);
    void show();
    void hide();
    void destroy();
};

#endif