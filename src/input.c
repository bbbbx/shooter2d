#include "input.h"

void doKeyDown(SDL_KeyboardEvent *event)
{
    if (0 == event->repeat)
    {
        if (SDL_SCANCODE_UP == event->keysym.scancode)
        {
            app.up = 1;
        }

        
        if (SDL_SCANCODE_DOWN == event->keysym.scancode)
        {
            app.down = 1;
        }

        if (SDL_SCANCODE_LEFT == event->keysym.scancode)
        {
            app.left = 1;
        }

        if (SDL_SCANCODE_RIGHT == event->keysym.scancode)
        {
            app.right = 1;
        }

        if (SDL_SCANCODE_SPACE == event->keysym.scancode)
        {
            app.fire = 1;
        }
    }
}

void doKeyUp(SDL_KeyboardEvent *event)
{
    if (0 == event->repeat)
    {
        if (SDL_SCANCODE_UP == event->keysym.scancode)
        {
            app.up = 0;
        }

        
        if (SDL_SCANCODE_DOWN == event->keysym.scancode)
        {
            app.down = 0;
        }

        if (SDL_SCANCODE_LEFT == event->keysym.scancode)
        {
            app.left = 0;
        }

        if (SDL_SCANCODE_RIGHT == event->keysym.scancode)
        {
            app.right = 0;
        }

        if (SDL_SCANCODE_SPACE == event->keysym.scancode)
        {
            app.fire = 0;
        }
    }
}

void doInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;

        case SDL_KEYDOWN:
            doKeyDown(&event.key);
            break;

        case SDL_KEYUP:
            doKeyUp(&event.key);
            break;
        
        default:
            break;
        }
    }
}