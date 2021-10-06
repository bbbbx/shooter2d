#include "text.h"

extern SDL_Texture* loadTexture(char *filename);
extern void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);

void initFonts()
{
    fontTexture = loadTexture((char*)"gfx/kromasky_16x16.png");
}

void drawText(int x, int y, int r, int g, int b, char *format, ...)
{
    int i, len, c;
    SDL_Rect rect;
    va_list args;

    memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

    va_start(args, format);
    vsprintf(drawTextBuffer, format, args);
    va_end(args);

    len = strlen(drawTextBuffer);

    rect.w = GLYPH_WIDTH;
    rect.h = GLYPH_HEIGHT;
    rect.y = 0;

    SDL_SetTextureColorMod(fontTexture, r, g, b);

    for (i= 0; i < len; i++)
    {
        c = drawTextBuffer[i];

        // ' ' : 32
        // 'z' : 122
        if (c >= ' ' && c <= 'z')
        {
            rect.x = (c - ' ') * GLYPH_WIDTH;

            blitRect(fontTexture, &rect, x, y);

            x += GLYPH_WIDTH;
        }
    }
}