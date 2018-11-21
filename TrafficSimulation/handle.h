#ifndef HANDLE_H
#define HANDLE_H

#include <SDL.h>
#include <vector>

#include "display.h"

struct Handle {
    SDL_Event ev;
    bool running = true;
    const Uint8 *keystate = nullptr;

    void loop();
    virtual void init_update() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void events() = 0;
};

#endif
