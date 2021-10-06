#pragma once

typedef struct {
    void (*logic)(void);
    void (*draw)(void);
} Delegate;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;

    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
} App;

struct Entity
{
    int x;
    int y;
    int w;
    int h;
    SDL_Texture *texture;

    int dx;
    int dy;
    int health;
    int reload;
    struct Entity *next;

    int side;
};

struct Explosion
{
    float x;
    float y;
    float dx;
    float dy;
    int r, g, b, a;
    struct Explosion *next;
};

struct Debris
{
    float x;
    float y;
    float dx;
    float dy;
    SDL_Rect rect;
    SDL_Texture *texture;
    int life;
    struct Debris *next;
};

struct Star
{
    int x;
    int y;
    int speed;
};

typedef struct {
    struct Entity fighterHead, *fighterTail;
    struct Entity bulletHead, *bulletTail;
    struct Entity pointsHead, *pointsTail;
    struct Explosion explosionHead, *explosionTail;
    struct Debris debrisHead, *debrisTail;
    int score;
} Stage;
