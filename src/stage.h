#pragma once

#include "common.h"

extern void blit(SDL_Texture*, int, int);
extern SDL_Texture* loadTexture(const char*);

extern App app;
extern Stage stage;
