#pragma once

#include "common.h"

extern void blit(SDL_Texture* texture, int x, int y);
extern SDL_Texture* loadTexture(const char* filename);
extern int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
extern void calcSlope(int x1, int y1, int x2, int y2, int *dx, int *dy);

extern App app;
extern Stage stage;
