#include "pch.h"
#include "handle.h"

void Handle::loop()
{
    while (running)
    {
        keystate = SDL_GetKeyboardState(NULL);
        init_update();

        if (keystate[SDL_SCANCODE_ESCAPE])
            running = false;

        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                running = false;

            events();
        }

        update();
        render();
    }
}