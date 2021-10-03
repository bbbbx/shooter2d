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

typedef struct {
    struct Entity fighterHead, *fighterTail;
    struct Entity bulletHead, *bulletTail;
} Stage;
