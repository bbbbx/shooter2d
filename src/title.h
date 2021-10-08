#pragma once

#include "common.h"

extern void initStage(void);
extern void initHighscores(void);
extern void doBackground(void);
extern void doStarfield(void);
extern void drawBackground(void);
extern void drawStarfield(void);
extern SDL_Texture* loadTexture(const char *filename);
extern void drawText(int x, int y, int r, int g, int b, int align, char *format, ...);
extern void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);

extern App app;
SDL_Texture *sdl2Texture;
SDL_Texture *shooterTexture;
int timeout;
int reveal;
