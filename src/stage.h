#pragma once

#include "common.h"

extern void blit(SDL_Texture* texture, int x, int y);
extern void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);
extern SDL_Texture* loadTexture(const char* filename);
extern int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
extern void calcSlope(int x1, int y1, int x2, int y2, int *dx, int *dy);
extern void playSound(int id, int channel);
extern void drawText(int x, int y, int r, int g, int b, char *format, ...);

extern void addHighscore(int score);
extern void initHighscores();

void initStage(void);
void doBackground(void);
void doStarfield(void);
void drawBackground(void);
void drawStarfield(void);


extern App app;
extern Stage stage;
