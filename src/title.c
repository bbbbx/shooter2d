#include "title.h"

static void logic()
{
    doBackground();

    doStarfield();

    if (reveal < SCREEN_HEIGHT)
    {
        reveal++;
    }

    if (--timeout <= 0)
    {
        initHighscores();
    }

    if (app.keyboard[SDL_SCANCODE_SPACE])
    {
        initStage();
    }
}

static void drawLogo()
{
    SDL_Rect rect;

    rect.x = 0;
    rect.y = 0;

    SDL_QueryTexture(sdl2Texture, NULL, NULL, &rect.w, &rect.h);
    rect.h = MIN(reveal, rect.h);
    blitRect(sdl2Texture, &rect, (SCREEN_WIDTH / 2) - (rect.w / 2), 100);

    SDL_QueryTexture(shooterTexture, NULL, NULL, &rect.w, &rect.h);
    rect.h = MIN(reveal, rect.h);
    blitRect(shooterTexture, &rect, (SCREEN_WIDTH / 2) - (rect.w / 2), 250);
}

static void draw()
{
    drawBackground();

    drawStarfield();

    drawLogo();

    if (timeout % 40 < 20)
    {
        drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, TEXT_CENTER, "PRESS SPACE TO FIRE!");
    }
}

void initTitle()
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

    sdl2Texture = loadTexture((char*)"gfx/sdl2.png");
    shooterTexture = loadTexture((char*)"gfx/shooter.png");

    timeout = FPS * 5;
}
