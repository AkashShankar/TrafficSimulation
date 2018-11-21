#include "pch.h"
#include "display.h"

void Display::init(std::string title, int w, int h)
{
    window_width = w;
    window_height = h;

    win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           w, h, SDL_WINDOW_SHOWN);

    win_id = SDL_GetWindowID(win);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Display::show()
{
    SDL_RaiseWindow(win);
}

void Display::hide()
{
    SDL_HideWindow(win);
}

void Display::destroy()
{
    if (ren)
        SDL_DestroyRenderer(ren);
    if (win)
        SDL_DestroyWindow(win);
}