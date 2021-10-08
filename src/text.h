#pragma once

#include "common.h"

void drawText(int x, int y, int r, int g, int b, int align, char *format, ...);

SDL_Texture *fontTexture;
char drawTextBuffer[MAX_LINE_LENGTH];
