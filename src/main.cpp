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

App app;
Entity player;
Entity bullet;

void loop()
{
    doInput();

    prepareScene();

// emulate velocity
    if (app.up) {
        player.y -= 4;
    }
    if (app.down) {
        player.y += 4;
    }
    if (app.left) {
        player.x -= 4;
    }
    if (app.right) {
        player.x += 4;
    }

    if (app.fire && bullet.health == 0)
    {
        bullet.x = player.x;
        bullet.y = player.y;
        bullet.dx = 16;
        bullet.dy = 0;
        bullet.health = 1;
    }

    bullet.x += bullet.dx;
    bullet.y += bullet.dy;

    if (bullet.x > SCREEN_WIDTH)
    {
        bullet.health = 0;
    }

    blit(player.texture, player.x, player.y);

    if (bullet.health > 0)
    {
        blit(bullet.texture, bullet.x, bullet.y);
    }

    presentScene();

    SDL_Delay(16);
}

int main(int argc, char** argv)
{
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));

    initSDL();

    player.x = 100;
    player.y = 100;
    player.texture = loadTexture((char*)"gfx/player.png");

    bullet.texture = loadTexture((char*)"gfx/playerBullet.png");

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