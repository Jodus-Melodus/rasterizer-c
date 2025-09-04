#include "screen.h"
#include <stdlib.h>
#include <string.h>

void initScreenBuffer(struct ScreenBuffer *sb)
{
    if (sb == NULL)
        return;
    sb->buffer = malloc(BUFFER_SIZE * sizeof(unsigned char));
    if (sb->buffer)
    {
        memset(sb->buffer, 0, BUFFER_SIZE);
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

const unsigned char get(const struct ScreenBuffer *sb, int x, int y)
{
    if (sb == NULL)
        return 0;
    int index = getIndex(x, y);
    if (index == -1)
        return 0;

    return sb->buffer[index];
}

void set(struct ScreenBuffer *sb, int x, int y, char value)
{
    if (sb == NULL)
        return;
    int index = getIndex(x, y);

    if (index == -1)
        return;

    sb->buffer[index] = value;
}

const char *display(struct ScreenBuffer *sb)
{
    if (sb == NULL)
        return 0;

    char *result = malloc(BUFFER_SIZE + HEIGHT + 1);
    if (!result)
        return NULL;

    size_t pos = 0;

    for (int y = -Y_OFFSET; y < HEIGHT - Y_OFFSET; y++)
    {
        for (int x = X_OFFSET; x < WIDTH - X_OFFSET; x++)
        {
            result[pos++] = get(sb, x, y) ? '?' : ' ';
        }
        result[pos++] = '\n';
    }
    result[pos] = '\0';
    return result;
}