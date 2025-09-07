#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "screen.h"

void initScreenBuffer(ScreenBuffer *sb)
{
    if (sb == NULL)
        return;
    sb->buffer = malloc(BUFFER_SIZE * sizeof(Color));
    if (sb->buffer)
        memset(sb->buffer, 0, BUFFER_SIZE * sizeof(Color));
}

void freeScreenBuffer(ScreenBuffer *sb)
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

const Color get(const ScreenBuffer *sb, int x, int y)
{
    if (sb == NULL)
        return (Color){.r = 0, .g = 0, .b = 0};
    int index = getIndex(x, y);
    if (index == -1)
        return (Color){.r = 0, .g = 0, .b = 0};

    return sb->buffer[index];
}

void set(ScreenBuffer *sb, int x, int y, Color color)
{
    if (sb == NULL)
        return;
    int index = getIndex(x, y);

    if (index == -1)
        return;

    sb->buffer[index] = color;
}

const char *display(const ScreenBuffer *sb)
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

void drawTriangle(ScreenBuffer *sb, Vector2 a, Vector2 b, Vector2 c, Color color)
{
    int maxX = (int)ceil(max(a.x, max(b.x, c.x)));
    int minX = (int)floor(min(a.x, min(b.x, c.x)));
    int maxY = (int)ceil(max(a.y, max(b.y, c.y)));
    int minY = (int)floor(min(a.y, min(b.y, c.y)));

    for (int y = minY; y < maxY; y++)
    {
        for (int x = minX; x < maxX; x++)
        {
            Vector2 p = {x, y};

            if (calculateBarycentricCoordinates(p, a, b, c) == 1)
                set(sb, x, y, color);
        }
    }
}

int calculateBarycentricCoordinates(Vector2 p, Vector2 a, Vector2 b, Vector2 c)
{
    float denominator = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
    float u = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denominator;
    float v = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denominator;
    float w = 1.0 - u - v;
    return (u >= 0.0) && (v >= 0.0) && (w >= 0.0);
}

Vector2 *projectCoordinate(const Vector3 p, const float focalLength)
{
    float denominator = focalLength + p.z;
    if (denominator == 0.0)
        denominator = 0.00001;
    float projectedX = (focalLength * p.x) / denominator;
    float projectedY = (focalLength * p.y) / denominator;
    Vector2 *projected = malloc(sizeof(Vector2));
    if (projected == NULL)
    {
        perror("Allocation failed");
        return NULL;
    }
    projected->x = projectedX;
    projected->y = projectedY;
    return projected;
}

int clearScreenBuffer(ScreenBuffer *sb)
{
    if (sb == NULL)
        return 1;
    sb->buffer = malloc(BUFFER_SIZE * sizeof(Color));
    if (sb->buffer == NULL)
        return 1;
    memset(sb->buffer, 0, BUFFER_SIZE * sizeof(Color));
    return 0;
}
