#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "screen.h"
#include "utils.h"

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

void drawTriangle(struct ScreenBuffer *sb, struct Vector2 a, struct Vector2 b, struct Vector2 c, struct Color color)
{
    int maxX = (int)ceil(max(a.data[0], max(b.data[0], c.data[0])));
    int minX = (int)floor(min(a.data[0], min(b.data[0], c.data[0])));
    int maxY = (int)ceil(max(a.data[1], max(b.data[1], c.data[1])));
    int minY = (int)floor(min(a.data[1], min(b.data[1], c.data[1])));

    for (int y = minY; y < maxY; y++)
    {
        for (int x = minX; x < maxX; x++)
        {
            struct Vector2 p = {{x, y}};

            if (calculateBarycentricCoordinates(p, a, b, c) == 1)
            {
                set(sb, x, y, color);
            }
        }
    }
}

int calculateBarycentricCoordinates(struct Vector2 p, struct Vector2 a, struct Vector2 b, struct Vector2 c)
{
    float denominator = (b.data[1] - c.data[1]) * (a.data[0] - c.data[0]) + (c.data[0] - b.data[0]) * (a.data[1] - c.data[1]);
    float u = ((b.data[1] - c.data[1]) * (p.data[0] - c.data[0]) + (c.data[0] - b.data[0]) * (p.data[1] - c.data[1])) / denominator;
    float v = ((c.data[1] - a.data[1]) * (p.data[0] - c.data[0]) + (a.data[0] - c.data[0]) * (p.data[1] - c.data[1])) / denominator;
    float w = 1.0 - u - v;
    return (u >= 0.0) && (v >= 0.0) && (w >= 0.0);
}