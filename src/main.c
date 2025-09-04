#include "screen.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    struct ScreenBuffer screen;
    initScreenBuffer(&screen);

    set(&screen, 0, 0, (struct Color){.r = 255, .g = 255, .b = 255});

    for (int y = -Y_OFFSET; y < HEIGHT - Y_OFFSET; y++)
    {
        for (int x = -X_OFFSET; x < WIDTH - X_OFFSET; x++)
        {
            float tx = (float)(x + X_OFFSET) / (WIDTH - 1);
            float ty = (float)(y + Y_OFFSET) / (HEIGHT - 1);
            float t = (tx + ty) / 2.0f;
            set(&screen, x, y, (struct Color){.r = (unsigned char)(tx * 255), .g = (unsigned char)(ty * 255), .b = (unsigned char)((1.0 - t) * 255)});
        }
    }

    const char *output = display(&screen);
    printf("%s", output);
    free((void *)output);
    freeScreenBuffer(&screen);

    return 0;
}