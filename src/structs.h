typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;

    // These variables will track our movement requests.
    int up;
    int down;
    int left;
    int right;

    int fire;
} App;

typedef struct
{
    int x;
    int y;
    SDL_Texture *texture;

    int dx;
    int dy;
    int health;
} Entity;

