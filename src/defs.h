#pragma once

#define PLAYER_SPEED 4
#define PLAYER_BULLET_SPEED 16
#define MAX_KEYBOARD_KEYS 350

#define SIDE_PLAYER 0
#define SIDE_ENEMY 1

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define FPS 60
#define ENEMY_BULLET_SPEED 8

#define MAX_STARS 20

#define MAX_SND_CHANNELS 8

// CH_ enum will specify the channel through which a sound will play
enum
{
    CH_ANY = -1,
    CH_PLAYER,
    CH_ENEMY_FIRE,
    CH_POINTS
};

// SND_ will be used to identify a sound effect
enum
{
    SND_PLAYER_FIRE,
    SND_ENEMY_FIRE,
    SND_PLAYER_DIE,
    SND_ENEMY_DIE,
    SND_POINTS,
    SND_MAX
};

#define MAX_LINE_LENGTH 1024
#define GLYPH_WIDTH 18
#define GLYPH_HEIGHT 28

#define NUM_HIGHSCORES 8

#define STRNCPY(dest, src, n) strncpy(dest, src, n); dest[n - 1] = '\0'

#define MAX_NAME_LENGTH 256

enum
{
    TEXT_LEFT,
    TEXT_CENTER,
    TEXT_RIGHT
};

#define MAX_SCORE_NAME_LENGTH 16