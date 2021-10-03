#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include "common.h"

extern "C" void doInput(void);
extern "C" void initSDL(void);
extern "C" void prepareScene(void);
extern "C" void presentScene(void);
extern "C" SDL_Texture* loadTexture(char *filename);
extern "C" void blit(SDL_Texture *texture, int x, int y);

extern "C" void initStage(void);

App app;
Stage stage;

long then;
float timeRemainder;

static void capFrameRate(long *then, float *remainder)
{
    long wait, frameTime;

    wait = 16 + *remainder;

    *remainder -= (int)*remainder;

    frameTime = SDL_GetTicks() - *then;

    wait -= frameTime;

    if (wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    *remainder += 0.667;

    *then = SDL_GetTicks();
}

void loop()
{
    prepareScene();

    doInput();

    app.delegate.logic();

    app.delegate.draw();

    presentScene();

    // SDL_Delay(16);
    capFrameRate(&then, &timeRemainder);
}

int main(int argc, char** argv)
{
    long then;
    float remainder;

    memset(&app, 0, sizeof(App));

    initSDL();

    // atexit(cleanup);

    initStage();

    then = SDL_GetTicks();

    remainder = 0;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(&loop, 0, 1);
    // emscripten_set_main_loop_timing(EM_TIMING_RAF, 1);
#else
    while (1)
    {
        loop();
    }
#endif

    return 0;
}