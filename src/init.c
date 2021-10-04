#include "init.h"

void initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    int windowFlags = 0;
    app.window = SDL_CreateWindow(
        "Title",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        windowFlags
    );

    if (!app.window)
    {
        printf("Failed to open %dx%d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "Linear");

    int rendererFlags = SDL_RENDERER_ACCELERATED;
    // The -1 tells SDL to use the first graphics acceleration device it finds.
    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (!app.renderer)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    #ifndef __EMSCRIPTEN__
    // See https://github.com/emscripten-ports/SDL2_image/issues/3
    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
    {
        printf("Failed to initialize SDL_image: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_ShowCursor(0);

    #endif
}