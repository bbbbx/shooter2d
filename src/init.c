#include "init.h"

void cleanup()
{
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}

void initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    int windowFlags = 0;
    app.window = SDL_CreateWindow(
        "Shoot'em Up",
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
// Emscripten doesn't need to call IMG_Init. See:
//   - https://github.com/emscripten-ports/SDL2_image/issues/3
//   - https://github.com/emscripten-core/emscripten/pull/3831#issuecomment-154383178
    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
    {
        printf("Failed to initialize SDL_image: %s\n", SDL_GetError());
        exit(1);
    }
#endif

    int frequency = 44100;
#ifdef __EMSCRIPTEN__
    frequency = EM_ASM_INT_V({
        var context;
        try {
            context = new AudioContext();
        } catch (e) {
            context = new webkitAudioContext(); // safari only
        }
        return context.sampleRate;
    });
#endif

    if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        printf("Couldn't initialize SDL Mixer\n");
        exit(1);
    }

    Mix_AllocateChannels(MAX_SND_CHANNELS);

    SDL_ShowCursor(0);
}

void initBackground()
{
    backgroundTexture = loadTexture((char*)"gfx/background.jpg");
}

void initStarfield()
{
    int i;

    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;
        stars[i].speed = rand() % 8 + 1;
    }
}

void initGame()
{
    initBackground();

    initStarfield();

    initFonts();

    initSounds();

    loadMusic("sound/324252__rhodesmas__rings-of-saturn-music-loop.wav");

    playMusic(1);

    initHighscoresTable();
}