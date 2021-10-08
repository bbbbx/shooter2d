#pragma once

#include "common.h"
#include "stage.h"

extern void initStage(void);
extern void doBackground(void);
extern void doStarfield(void);
extern void drawBackground(void);
extern void drawStarfield(void);

void initHighscoresTable(void);
void initHighscores(void);

Highscores highscores;
Highscore *newHighscore;
int cursorBlink = 0;