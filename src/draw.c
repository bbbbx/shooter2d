#include "draw.h"

void prepareScene()
{
    SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene()
{
    SDL_RenderPresent(app.renderer);
}

static SDL_Texture *getTexture(char *name)
{
    struct Texture *t;

    for (t = app.textureHead.next; t != NULL; t = t->next)
    {
        if (strcmp(t->name, name) == 0)
        {
            return t->texture;
        }
    }

    return NULL;
}

static void addTextureToCache(char *name, SDL_Texture *sdlTexture)
{
    struct Texture *texture;

    texture = malloc(sizeof(struct Texture));
	memset(texture, 0, sizeof(struct Texture));

    STRNCPY(texture->name, name, MAX_NAME_LENGTH);
    texture->texture = sdlTexture;

    app.textureTail->next = texture;
    app.textureTail = texture;
}

SDL_Texture* loadTexture(char *filename)
{
    SDL_Texture *texture;

    texture = getTexture(filename);

    if (!texture) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

        texture = IMG_LoadTexture(app.renderer, filename);

        if (!texture) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Loading %s failed", filename);
            exit(1);
        }

        addTextureToCache(filename, texture);
    }

    return texture;
}

void blit(SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = src->w;
    dest.h = src->h;

    SDL_RenderCopy(app.renderer, texture, src, &dest);
}

void drawBackground()
{
    SDL_Rect dest;
    int x;

    for (x = backgroundX; x < screenWidth; x += screenWidth)
    {
        dest.x = x;
        dest.y = 0;
        dest.w = screenWidth;
        dest.h = screenHeight;

        SDL_RenderCopy(app.renderer, backgroundTexture, NULL, &dest);
    }
}

void drawStarfield()
{
    int i, c;

    for (i = 0; i < MAX_STARS; i++)
    {
        c = stars[i].speed * 32;

        SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

        SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
    }
}