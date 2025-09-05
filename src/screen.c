#include <stdlib.h>
#include <string.h>
#include "screen.h"

void initScreenBuffer(struct ScreenBuffer *sb)
{
    if (sb == NULL)
        return;
    sb->buffer = malloc(BUFFER_SIZE * sizeof(struct Color));
    if (sb->buffer)
    {
        memset(sb->buffer, 0, BUFFER_SIZE * sizeof(struct Color));
    }
}

void freeScreenBuffer(struct ScreenBuffer *sb)
{
    if (sb == NULL)
        return;
    free(sb->buffer);
    sb->buffer = NULL;
}

int getIndex(int x, int y)
{
    int xi = x + X_OFFSET;
    int yi = y + Y_OFFSET;

    if (xi < 0 || xi >= WIDTH || yi < 0 || yi >= HEIGHT)
        return -1;

    return WIDTH * yi + xi;
}

const struct Color get(const struct ScreenBuffer *sb, int x, int y)
{
    if (sb == NULL)
        return (struct Color){.r = 0, .g = 0, .b = 0};
    int index = getIndex(x, y);
    if (index == -1)
        return (struct Color){.r = 0, .g = 0, .b = 0};

    return sb->buffer[index];
}

void set(struct ScreenBuffer *sb, int x, int y, struct Color color)
{
    if (sb == NULL)
        return;
    int index = getIndex(x, y);

    if (index == -1)
        return;

    sb->buffer[index] = color;
}

const char *display(const struct ScreenBuffer *sb)
{
    if (sb == NULL)
        return 0;

    char *result = malloc(BUFFER_SIZE + HEIGHT + 1);
    if (!result)
        return NULL;

    size_t pos = 0;

    for (int y = -Y_OFFSET; y < HEIGHT - Y_OFFSET; y++)
    {
        for (int x = -X_OFFSET; x < WIDTH - X_OFFSET; x++)
        {
            unsigned char gray = toGray(get(sb, x, y));
            int gradientIndex = gray * (sizeof(GRADIENT) - 1) / 255;
            result[pos++] = GRADIENT[gradientIndex];
        }
        result[pos++] = '\n';
    }
    result[pos] = '\0';
    return result;
}