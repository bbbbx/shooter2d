#pragma once

#include "common.h"

#include <SDL2/SDL_image.h>

void blit(SDL_Texture *texture, int x, int y);
void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);

extern App app;
extern SDL_Texture *backgroundTexture;
extern struct Star stars[MAX_STARS];
extern int backgroundX;
extern int screenWidth;
extern int screenHeight;