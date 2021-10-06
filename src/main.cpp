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
extern "C" void initHighscores(void);
extern "C" void initSounds(void);
extern "C" void initFonts(void);

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
    SDL_version version;
    SDL_GetVersion(&version);
    printf("Linked against SDL %d.%d.%d.\n",
           version.major, version.minor, version.patch);
    assert(version.major == 2);

    long then;
    float remainder;

    memset(&app, 0, sizeof(App));

    initSDL();

    // atexit(cleanup);

    initStage();

    initHighscores();

    initSounds();

    initFonts();

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