#pragma once

#include "common.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

extern void initFonts(void);
extern void initSounds(void);
extern void initHighscoresTable(void);
extern void loadMusic(const char *filename);
extern void playMusic(int loop);
extern SDL_Texture* loadTexture(const char *filename);

extern App app;
extern SDL_Texture *backgroundTexture;
extern struct Star stars[MAX_STARS];
extern int backgroundX;

void initSDL(void);
void initGame(void);
void initBackground(void);
void initStarfield(void);
