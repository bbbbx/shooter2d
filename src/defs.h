#pragma once

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

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
    CH_ENEMY_FIRE
};

// SND_ will be used to identify a sound effect
enum
{
    SND_PLAYER_FIRE,
    SND_ENEMY_FIRE,
    SND_PLAYER_DIE,
    SND_ENEMY_DIE,
    SND_MAX
};

#define MAX_LINE_LENGTH 1024
#define GLYPH_WIDTH 16
#define GLYPH_HEIGHT 16